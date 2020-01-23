#include "RenderSystem.h"

#include "LoadFNT.h"
#include "LoadOBJ.h"

#include "Framebuffer.h"

#include "Transform.h"

#include <Events/EventsManager.h>
#include <Logger/Logger.h>
#include <Math/Math.hpp>
#include <Math/MatrixTransform.hpp>

#include <GL/glew.h>

#include <algorithm>

RenderSystem::~RenderSystem() {
	cameras.clear();
	lights.clear();
	components.clear();

	delete depthShader;

	for (unsigned i = 0; i < MAX_LIGHTS; ++i) {
		delete depthFBO[i];
	}

	delete mainFBO;
	delete blurPass;
	delete finalBloomPass;

	delete textShader;

	delete depthRenderer;
	delete blurRenderer;
	delete posterizeRenderer;
	delete bloomRenderer;
}

void RenderSystem::Initialize() {
	lights.reserve(MAX_LIGHTS);

	if (instanceBuffer == 0)
		glGenBuffers(1, &instanceBuffer);

	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//quad = Load::OBJ("Files/Models/quad.obj");

	depthShader = new Shader("Files/Shaders/simple.vert", "Files/Shaders/simple.frag");

	{
		TextureData tData;
		tData.level = 0;
		tData.internalFormat = GL_DEPTH_COMPONENT;
		tData.border = 0;
		tData.format = GL_DEPTH_COMPONENT;
		tData.type = GL_FLOAT;
		tData.attachment = GL_DEPTH_ATTACHMENT;
		tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
		tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
		tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER });
		tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER });

		for (unsigned i = 0; i < MAX_LIGHTS; ++i) {
			depthFBO[i] = new Framebuffer(1, 0);
			depthFBO[i]->Initialize(vec2u(1000, 1000), { tData }, { });
		}
	}

	{
		std::vector<TextureData> tDataList;

		for (int i = 0; i < 2; ++i) {
			TextureData tData;
			tData.level = 0;
			tData.internalFormat = GL_RGB16F;
			tData.border = 0;
			tData.format = GL_RGBA;
			tData.type = GL_UNSIGNED_BYTE;
			tData.attachment = GL_COLOR_ATTACHMENT0 + i;
			tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
			tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
			tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE });
			tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE });
			tDataList.push_back(tData);
		}

		RenderBufferData rbData;
		rbData.internalFormat = GL_DEPTH24_STENCIL8;
		rbData.attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

		mainFBO = new Framebuffer(2, 1);
		mainFBO->Initialize(vec2u(1600, 900), tDataList, { rbData });
	}

	{
		TextureData tData;
		tData.level = 0;
		tData.internalFormat = GL_RGB16F;
		tData.border = 0;
		tData.format = GL_RGBA;
		tData.type = GL_UNSIGNED_BYTE;
		tData.attachment = GL_COLOR_ATTACHMENT0;
		tData.parameters.push_back({ GL_TEXTURE_MIN_FILTER, GL_LINEAR });
		tData.parameters.push_back({ GL_TEXTURE_MAG_FILTER, GL_LINEAR });
		tData.parameters.push_back({ GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE });
		tData.parameters.push_back({ GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE });

		RenderBufferData rbData;
		rbData.internalFormat = GL_DEPTH24_STENCIL8;
		rbData.attachmentFormat = GL_DEPTH_STENCIL_ATTACHMENT;

		blurPass = new Framebuffer(1, 1);
		blurPass->Initialize(vec2u(1600, 900), { tData }, { rbData });

		finalBloomPass = new Framebuffer(1, 1);
		finalBloomPass->Initialize(vec2u(1600, 900), { tData }, { rbData });
	}

	textShader = new Shader("Files/Shaders/nonlit.vert", "Files/Shaders/textured.frag");

	depthRenderer = new Renderer::FBO("Files/Shaders/fb.vert", "Files/Shaders/depth.frag");
	blurRenderer = new Renderer::FBO("Files/Shaders/fb.vert", "Files/Shaders/blur.frag");
	posterizeRenderer = new Renderer::FBO("Files/Shaders/fb.vert", "Files/Shaders/posterize.frag");
	bloomRenderer = new Renderer::Additive("Files/Shaders/fb.vert", "Files/Shaders/bloom.frag");

	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("LIGHT_ACTIVE", &RenderSystem::LightActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &RenderSystem::RenderActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_ACTIVE", &RenderSystem::TextActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_FONT", &RenderSystem::TextFontHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &RenderSystem::ResizeHandle, this);
	Events::EventsManager::GetInstance()->Subscribe("DEBUG_TEXT", &RenderSystem::DebugTextHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("FRUSTRUM_CULL", &RenderSystem::FrustrumCullHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("PARTITION", &RenderSystem::PartitionHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("LOD", &RenderSystem::LODHandler, this);
}

void RenderSystem::Update(const float& t) {
	//Events::EventsManager::GetInstance()->Trigger("TIMER_START", new Events::AnyType<std::string>("RENDER"));
	if (cameras.empty()) return;

	Batch();

	indicesCount = 0;
	cullCount = 0;

	glCullFace(GL_FRONT);

	//Events::EventsManager::GetInstance()->Trigger("TIMER_START", new Events::AnyType<std::string>("DEPTH MAP"));
	for (unsigned i = 0; i < lights.size(); ++i) {
		const auto size = depthFBO[i]->GetSize();
		glViewport(0, 0, size.w, size.h);
		glScissor(0, 0, size.w, size.h);

		const auto& light = lights[i];

		if (light->type == Light::POINT) continue;

		depthShader->Use();

		const auto& lightProjection = light->GetProjectionMatrix();
		const auto& lightLookAt = entities->GetComponent<Transform>(light->entity)->GetLocalLookAt();
		const auto& lightSpaceMatrix = lightProjection * lightLookAt;

		depthShader->SetMatrix4("projection", lightProjection);
		depthShader->SetMatrix4("view", lightLookAt);

		depthFBO[i]->Bind();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& shaderBatch : batches[""]) {
			for (auto& materialBatch : shaderBatch.second) {
				RenderWorld(materialBatch.second);
			}
		}

		depthFBO[i]->Unbind();

		light->shadowMap = depthFBO[i]->GetTexture();
		lightSpaceMatrices[i] = lightSpaceMatrix;
	}
	//Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("DEPTH MAP"));

	glCullFace(GL_BACK);
	//Events::EventsManager::GetInstance()->Trigger("TIMER_START", new Events::AnyType<std::string>("MAIN"));

	for (const auto& cam : cameras) {
		const auto& viewport = cam->GetViewport();
		const auto& projection = cam->GetProjectionMatrix();

		const auto& transform = entities->GetComponent<Transform>(cam->entity);
		const auto& viewPosition = transform->GetWorldTranslation();
		const auto& lookAt = transform->GetLocalLookAt();

		const Math::vec2<GLint> origin(
			static_cast<GLint>(viewport.origin.x),
			static_cast<GLint>(viewport.origin.y)
		);

		const Math::vec2<GLint> size(
			static_cast<GLsizei>(viewport.size.w),
			static_cast<GLsizei>(viewport.size.h)
		);

		if (cam->flags & BLOOM_BIT) {
			mainFBO->Resize(size);
			mainFBO->Bind();
		}

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		if (cam->clear) {
			glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		if (cam->groups.empty()) {
			for (auto& shaderBatch : batches[""]) {
				auto shader = shaderBatch.first;
				shader->Use();
				shader->SetFloat("near", cam->nearPlane);
				shader->SetFloat("far", cam->farPlane);

				shader->SetMatrix4("projection", projection);
				shader->SetMatrix4("view", lookAt);

				SetLightUniforms(viewPosition, shader);
				
				for (auto& materialBatch : shaderBatch.second) {
					materialBatch.first->SetAttributes();
					RenderWorld(materialBatch.second);
				}

				//if (quadBatch.size() > 0) {
				//	const auto mesh = quad->meshes[0];
				//	glBindVertexArray(mesh->VAO);

				//	const unsigned count = quadBatch.size();
				//	const unsigned unit = 4 * sizeof(float);
				//	const unsigned stride = sizeof(mat4f);

				//	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
				//	glBufferData(GL_ARRAY_BUFFER, count * stride, &quadBatch[0], GL_STATIC_DRAW);

				//	int start, i;
				//	start = i = 3;

				//	for (int u = 0; u < 4; ++u) {
				//		glEnableVertexAttribArray(i);
				//		glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(u * unit));
				//	}

				//	for (; start < i; ++start)
				//		glVertexAttribDivisor(start, 1);

				//	glDrawElementsInstanced(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, (void*)(0), count);
				//	indicesCount += mesh->indicesSize * count;
				//}
			}
		} else {
			for (auto& group : cam->groups) {
				for (auto& shaderBatch : batches[group]) {
					auto shader = shaderBatch.first;
					shader->Use();
					shader->SetFloat("near", cam->nearPlane);
					shader->SetFloat("far", cam->farPlane);

					shader->SetMatrix4("projection", projection);
					shader->SetMatrix4("view", lookAt);

					SetLightUniforms(viewPosition, shader);

					for (auto& materialBatch : shaderBatch.second) {
						materialBatch.first->SetAttributes();
						RenderWorld(materialBatch.second);
					}
				}
			}
		}

		

		if (cam->flags & BLOOM_BIT) {
			mainFBO->Unbind();

			//NOTE: Bloom code

			Framebuffer* fb[2] = { blurPass, finalBloomPass };

			bool horizontal = true, firstIteration = true;
			unsigned amount = 2;

			for (unsigned i = 0; i < amount; ++i) {
				fb[horizontal]->Bind();
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				unsigned curBindTexture = firstIteration ? mainFBO->GetTexture(1) : fb[!horizontal]->GetTexture();
				blurRenderer->PreRender();
				blurRenderer->GetShader()->SetInt("horizontal", horizontal);
				blurRenderer->Render(curBindTexture);

				horizontal = !horizontal;
				if (firstIteration)
					firstIteration = false;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			bloomRenderer->PreRender();
			bloomRenderer->GetShader()->SetFloat("exposure", 1.f);
			bloomRenderer->Render(mainFBO->GetTexture(), fb[!horizontal]->GetTexture());
		}
	}

	glViewport(0, 0, windowSize.w, windowSize.h);
	glScissor(0, 0, windowSize.w, windowSize.h);

	//uiShader->Use();
	//uiShader->SetMatrix4("projection", canvas->GetProjectionMatrix());
	//uiShader->SetMatrix4("view", canvasLookAt);

	depthRenderer->PreRender(vec3f(vec2f(0.9f), -1.f), vec2f(0.1f));
	depthRenderer->GetShader()->SetFloat("near", 0.1f);
	depthRenderer->GetShader()->SetFloat("far", 10.0f);
	depthRenderer->Render(depthFBO[0]->GetTexture());

	//fboRenderer.PreRender(vec3f(vec2f(0.9f), -1.f), vec2f(0.1f));
	//fboRenderer.Render(mainFBO->GetTexture(1));

	//Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("MAIN"));
	//Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("RENDER"));
}

void RenderSystem::CameraActiveHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i] == c) {
				return;
			} else if (cameras[i]->depth >= c->depth) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	} else {
		Helpers::Remove(cameras, c);
	}
}

void RenderSystem::CameraDepthHandler(Events::Event* event) {
	auto& c = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (c->IsActive()) {
		cameras.erase(vfind(cameras, c));
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= c->depth) {
				cameras.insert(cameras.begin() + i, c);
				return;
			}
		}
		cameras.push_back(c);
	}
}

void RenderSystem::LightActiveHandler(Events::Event* event) {
	const auto light = static_cast<Events::AnyType<Light*>*>(event)->data;

	if (light->IsActive()) {
		Helpers::Insert(lights, light);
	} else {
		Helpers::Remove(lights, light);
	}
}

void RenderSystem::RenderActiveHandler(Events::Event* event) { 
	const auto component = static_cast<Events::AnyType<Render*>*>(event)->data;

	if (component->IsActive()) {
		Helpers::Insert(components, component);
	} else {
		Helpers::Remove(components, component);
	}
}

void RenderSystem::TextActiveHandler(Events::Event * event) {
	auto& c = static_cast<Events::AnyType<Text*>*>(event)->data;
	const auto font = c->GetFont();
	if (!font) return;

	auto& textList = textBatches[font];

	if (c->IsActive()) {
		Helpers::Insert(textList, c);
	} else {
		Helpers::Remove(textList, c);
	}
}

void RenderSystem::TextFontHandler(Events::Event * event) {
	const auto change = static_cast<Events::FontChange*>(event);
	auto& c = change->component;
	auto previous = change->previous;
	auto current = c->GetFont();

	if (!previous) {
		textBatches[current].push_back(c);
	} else if (!current) {
		auto& textList = textBatches[previous];
		textList.erase(vfind(textList, c));
	} else {
		auto& prevList = textBatches[previous];
		prevList.erase(vfind(prevList, c));
		textBatches[current].push_back(c);
	}
}

void RenderSystem::ResizeHandle(Events::Event* event) {
	const auto size = static_cast<Events::AnyType<vec2i>*>(event)->data;
	windowSize = size;
	mainFBO->Resize(size);
	blurPass->Resize(size);
	finalBloomPass->Resize(size);
}

void RenderSystem::DebugTextHandler(Events::Event * event) {
	debugText = static_cast<Events::AnyType<std::string>*>(event)->data;
}

void RenderSystem::FrustrumCullHandler(Events::Event * event) {
	frustrumCull = static_cast<Events::AnyType<bool>*>(event)->data;
}

void RenderSystem::PartitionHandler(Events::Event * event) {
	partition = static_cast<Events::AnyType<bool>*>(event)->data;
}

void RenderSystem::LODHandler(Events::Event * event) {
	LOD = static_cast<Events::AnyType<bool>*>(event)->data;
}

void RenderSystem::Batch() {
	//quadBatch.clear();
	batches.clear();

	const auto cam = cameras[0];
	const auto camTransform = entities->GetComponent<Transform>(cam->entity);
	const auto camPosition = camTransform->GetWorldTranslation();
	const auto camDirection = camTransform->GetLocalFront();
	const auto theta = cos(Math::Rad(cam->FOV * 1.1f));
		
	//auto quad = camEntity->GetQuad();
	//while (quad->parent) {
	//	quad = quad->parent;
	//}

	if (partition) {
		//Traverse(quad, camPosition, camDirection, theta);
	} else {
		//mat4f transform;
		//Math::SetToTransform(transform, quad->position, vec3f(0.f), quad->size * 1.9f);
		//quadBatch.push_back(transform);

		for (auto& c : components) {
			auto transform = entities->GetComponent<Transform>(c->entity);

			auto point = transform->GetWorldTranslation();
			const auto radius = Math::Max(Math::Max(transform->scale.x, transform->scale.y), transform->scale.z);
			point += camDirection * radius;

			const auto diff = point - camPosition;
			const float depth = Math::Length(diff);
			const auto dir = diff / depth;

			if (frustrumCull) {
				++cullCount;
				if (Math::Dot(dir, camDirection) < theta) continue;
			}

			const auto matrix = transform->GetWorldTransform();
			auto model = c->model;

			if (LOD) {
				if (depth > 30.f) {
					if (c->lowModel)
						model = c->lowModel;
				} else if (depth > 10.f) {
					if (c->midModel)
						model = c->midModel;
				}
			}
			
			for (auto& mesh : model->meshes) {
				if (c->groups.empty()) {
					batches[""][c->material->GetShader()][c->material][mesh].push_back(matrix);
				} else {
					for (auto& group : c->groups) {
						batches[group][c->material->GetShader()][c->material][mesh].push_back(matrix);
					}
				}
			}
		}
	}

}

//void RenderSystem::Traverse(Quad<Entity*>* const quad, const vec3f & pos, const vec3f & dir, const float & theta) {
//	if (!quad->topLeft) {
//		auto point = quad->position;
//		const auto radius = Math::Max(quad->size.x, quad->size.z) * 2.f;
//		point += dir * radius;
//
//		const auto diff = point - pos;
//		const float depth = Math::Length(diff);
//		const auto quadDir = diff / depth;
//
//		if (frustrumCull) {
//			++cullCount;
//			if (Math::Dot(quadDir, dir) < theta) return;
//		}
//
//		{
//			mat4f transform;
//			Math::SetToTransform(transform, quad->position, vec3f(0.f), quad->size * 1.9f);
//			quadBatch.push_back(transform);
//		}
//
//		for (auto& list : quad->list) {
//			auto c = list->GetComponent<Render>();
//
//			if (!c || !c->IsActive()) continue;
//
//			auto transform = c->GetParent()->GetComponent<Transform>();
//
//			const auto matrix = transform->GetWorldTransform();
//
//			auto model = c->model;
//
//			if (LOD) {
//				if (depth > 30.f) {
//					if (c->lowModel)
//						model = c->lowModel;
//				} else if (depth > 10.f) {
//					if (c->midModel)
//						model = c->midModel;
//				}
//			}
//
//			for (auto& mesh : model->meshes) {
//				batches[c->material->GetShader()][c->material][mesh].push_back(matrix);
//			}
//		}
//	} else {
//		Traverse(quad->topLeft, pos, dir, theta);
//		Traverse(quad->topRight, pos, dir, theta);
//		Traverse(quad->bottomLeft, pos, dir, theta);
//		Traverse(quad->bottomRight, pos, dir, theta);
//	}
//}

void RenderSystem::SetLightUniforms(const vec3f& viewPosition, Shader * const shader) {
	const unsigned count = Math::Min(static_cast<unsigned>(lights.size()), MAX_LIGHTS);

	shader->SetInt("lightCount", count);
	shader->SetVector3("viewPosition", viewPosition);

	for (unsigned i = 0; i < count; ++i) {
		const auto& light = lights[i];
		const auto& transform = entities->GetComponent<Transform>(light->entity);

		const vec3f& position = transform->translation;
		const vec3f& direction = transform->GetLocalFront();

		const std::string tag = "lights[" + std::to_string(i) + "].";

		shader->SetInt(tag + "type", static_cast<int>(light->type));

		shader->SetVector3(tag + "position", position);
		shader->SetVector3(tag + "direction", direction);

		shader->SetVector3(tag + "ambient", light->ambient);
		shader->SetVector3(tag + "diffuse", light->diffuse);
		shader->SetVector3(tag + "specular", light->specular);

		shader->SetFloat(tag + "power", light->power);

		shader->SetFloat(tag + "constant", light->constant);
		shader->SetFloat(tag + "linear", light->linear);
		shader->SetFloat(tag + "quadratic", light->quadratic);

		shader->SetFloat(tag + "cutOff", light->GetCutOff());
		shader->SetFloat(tag + "outerCutOff", light->GetOuterCutOff());

		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, light->shadowMap);

		shader->SetMatrix4("lightSpaceMatrices[" + std::to_string(i) + ']', lightSpaceMatrices[i]);
	}
}

void RenderSystem::RenderWorld(MeshBatch & batch) {
	for (auto& meshBatch : batch) {
		const auto mesh = meshBatch.first;
		glBindVertexArray(mesh->VAO);

		const auto instances = meshBatch.second;
		const unsigned count = instances.size();
		const unsigned unit = 4 * sizeof(float);
		const unsigned stride = sizeof(mat4f);

		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, count * stride, &instances[0], GL_STATIC_DRAW);

		int start, i;
		start = i = 3;

		for (int u = 0; u < 4; ++u) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i++, 4, GL_FLOAT, GL_FALSE, stride, (void*)(u * unit));
		}

		for (; start < i; ++start)
			glVertexAttribDivisor(start, 1);

		glDrawElementsInstanced(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, (void*)(0), count);
		indicesCount += mesh->indicesSize * count;
	}
}

void RenderSystem::RenderText(TextBatches & textBatches) {
	for (auto& textPair : textBatches) {
		auto& font = textPair.first;
		if (!font) continue;

		const auto tex = font->texture;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		textShader->SetInt("useTex", tex);

		glBindVertexArray(font->VAO);

		for (auto& text : textPair.second) {
			textShader->SetVector4("color", text->color);
			auto transform = entities->GetComponent<Transform>(text->entity);

			const float scale = text->scale;

			std::vector<float> lineOffset;
			vec2f size(0.f);

			const auto& content = text->text;

			for (unsigned i = 0; i <= content.size(); ++i) {
				auto& c = content[i];

				switch (c) {
				case '\n':
				case '\0':
					++size.y;
					switch (text->paragraphAlignment) {
					case PARAGRAPH_CENTER:
						lineOffset.push_back(size.x * scale * 0.5f);
						break;
					case PARAGRAPH_RIGHT:
						lineOffset.push_back(transform->scale.x * -0.5f - size.x * -scale);
						break;
					default:
						lineOffset.push_back(transform->scale.x * 0.5f);
						break;
					}
					size.x = 0.f;
					break;
				default:
					size.x += font->characters[c].xAdvance;
					break;
				}
			}
			size.y *= font->lineHeight * text->lineSpacing * text->scale;

			const vec3f translation = transform->GetWorldTranslation();
			vec3f position(0.f);
			position.x = translation.x - lineOffset[0];

			switch (text->verticalAlignment) {
			case ALIGN_MIDDLE:
				position.y = translation.y + size.y * 0.5f;
				break;
			case ALIGN_BOTTOM:
				position.y = translation.y - transform->scale.y * 0.5f + size.y;
				break;
			default:
				position.y = translation.y + transform->scale.y * 0.5f;
				break;
			}

			int lineNumer = 0;
			for (auto& c : content) {
				if (c == '\0') continue;

				switch (c) {
				case '\n':
					position.y -= font->lineHeight * text->lineSpacing * text->scale;
					position.x = translation.x - lineOffset[++lineNumer];
					break;
				default:
					const auto& character = font->characters[c];
					const vec3f offset = character.rect.origin * scale;

					mat4f model;
					Math::SetToTranslation(model, position + offset);
					if (text->scale != 1.f)
						Math::Scale(model, vec3f(scale));
					textShader->SetMatrix4("model", model);

					const int index = character.index * 6;
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(index * sizeof(unsigned)));

					position.x += character.xAdvance * text->characterSpacing * scale;
					break;
				}
			}
		}
	}
}
