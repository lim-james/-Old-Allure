#include "RenderSystem.h"

#include "Load/LoadFNT.h"

#include "Framebuffer/Framebuffer.h"

#include "../Entity/Entity.h"
#include "../Components/Transform/Transform.h"

#include <Events/EventsManager.h>
#include <Logger/Logger.h>
#include <Math/Math.hpp>
#include <Math/MatrixTransform.hpp>

#include <GL/glew.h>

#include <algorithm>

RenderSystem::RenderSystem() {
	lights.reserve(MAX_LIGHTS);

	if (instanceBuffer == 0)
		glGenBuffers(1, &instanceBuffer);

	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	canvas = new Camera;
	canvas->projection = ORTHOGRAPHIC;
	canvas->size = 20.f;
		
	uiShader = new Shader("Files/Shaders/nonlit.vert", "Files/Shaders/textured.frag");

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
}

RenderSystem::~RenderSystem() {
	cameras.clear();
	lights.clear();
	components.clear();

	delete canvas;
	delete uiShader;

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

void RenderSystem::Start() {
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("LIGHT_ACTIVE", &RenderSystem::LightActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &RenderSystem::RenderActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &RenderSystem::ResizeHandle, this);
	Events::EventsManager::GetInstance()->Subscribe("DEBUG_TEXT", &RenderSystem::DebugTextHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("FRUSTRUM_CULL", &RenderSystem::FrustrumCullHandler, this);
}

void RenderSystem::Update(const float& t) {
	//Events::EventsManager::GetInstance()->Trigger("TIMER_START", new Events::AnyType<std::string>("RENDER"));
	if (cameras.empty()) return;

	indicesCount = 0;
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
		const auto& lightLookAt = light->GetParent()->GetComponent<Transform>()->GetLocalLookAt();
		const auto& lightSpaceMatrix = lightProjection * lightLookAt;

		depthShader->SetMatrix4("projection", lightProjection);
		depthShader->SetMatrix4("view", lightLookAt);

		depthFBO[i]->Bind();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& shaderBatch : batches) {
			for (auto& materialBatch : shaderBatch.second) {
				for (auto& meshBatch : materialBatch.second) {
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
		}

		depthFBO[i]->Unbind();

		light->shadowMap = depthFBO[i]->GetTexture();
		lightSpaceMatrices[i] = lightSpaceMatrix;
	}
	//Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("DEPTH MAP"));

	glCullFace(GL_BACK);
	//Events::EventsManager::GetInstance()->Trigger("TIMER_START", new Events::AnyType<std::string>("MAIN"));

	for (const auto& cam : cameras) {
		//mainFBO->Bind();

		const auto& viewport = cam->GetViewport();
		const auto& projection = cam->GetProjectionMatrix();
		const auto& lookAt = cam->GetParent()->GetComponent<Transform>()->GetLocalLookAt();

		const Math::vec2<GLint> origin(
			static_cast<GLint>(viewport.origin.x),
			static_cast<GLint>(viewport.origin.y)
		);

		const Math::vec2<GLint> size(
			static_cast<GLsizei>(viewport.size.w),
			static_cast<GLsizei>(viewport.size.h)
		);

		glViewport(origin.x, origin.y, size.x, size.y);
		glScissor(origin.x, origin.y, size.x, size.y);
		glClearColor(cam->clearColor.r, cam->clearColor.g, cam->clearColor.b, cam->clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& shaderBatch : batches) {
			auto shader = shaderBatch.first;
			shader->Use();
			shader->SetFloat("near", cam->nearPlane);
			shader->SetFloat("far", cam->farPlane);

			shader->SetMatrix4("projection", projection);
			shader->SetMatrix4("view", lookAt);

			SetLightUniforms(cam, shader);

			for (auto& materialBatch : shaderBatch.second) {
				materialBatch.first->SetAttributes();

				for (auto& meshBatch : materialBatch.second) {
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
		}
		//mainFBO->Unbind();

		// NOTE: Bloom code

		//Framebuffer* fb[2] = { blurPass, finalBloomPass };

		//bool horizontal = true, firstIteration = true;
		//unsigned amount = 2;

		//for (unsigned i = 0; i < amount; ++i) {
		//	fb[horizontal]->Bind();
		//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	unsigned curBindTexture = firstIteration ? mainFBO->GetTexture(1) : fb[!horizontal]->GetTexture();
		//	blurRenderer->PreRender();
		//	blurRenderer->GetShader()->SetInt("horizontal", horizontal);
		//	blurRenderer->Render(curBindTexture);

		//	horizontal = !horizontal;
		//	if (firstIteration)
		//		firstIteration = false;
		//}

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//bloomRenderer->PreRender();
		//bloomRenderer->GetShader()->SetFloat("exposure", 1.f);
		//bloomRenderer->Render(mainFBO->GetTexture(), fb[!horizontal]->GetTexture());
	}

	glViewport(0, 0, windowSize.w, windowSize.h);
	glScissor(0, 0, windowSize.w, windowSize.h);

	uiShader->Use();
	uiShader->SetMatrix4("projection", canvas->GetProjectionMatrix());
	uiShader->SetMatrix4("view", canvasLookAt);

	auto font = Load::FNT("Files/Fonts/Microsoft.fnt", "Files/Fonts/Microsoft.tga");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->texture);
	glBindVertexArray(font->mesh->VAO);

	vec3f position(0.f);
		
	for (auto c : debugText) {
		if (c == '\0') continue;
		switch (c) {
		case '\0':
			continue;
		case '\n':
			position.y -= font->lineHeight;
			position.x = 0.f;
			break;
		default:
			auto character = font->characters[c];

			const vec3f offset = character.rect.origin;

			mat4f model;
			Math::SetToTranslation(model, position + offset);
			uiShader->SetMatrix4("model", model);

			const int index = character.index * 6;
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(index * sizeof(unsigned)));

			position.x += character.xAdvance;
			break;
		}

	}

	//depthRenderer->PreRender(vec3f(vec2f(0.9f), -1.f), vec2f(0.1f));
	//depthrenderer->getshader()->setfloat("near", 0.1f);
	//depthrenderer->getshader()->setfloat("far", 10.0f);
	//depthrenderer->render(depthfbo[0]->gettexture());

	//fboRenderer.PreRender(vec3f(vec2f(0.9f), -1.f), vec2f(0.1f));
	//fboRenderer.Render(mainFBO->GetTexture(1));

	//Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("MAIN"));
	//Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("RENDER"));
}

void RenderSystem::FixedUpdate(const float & t) { 
	Batch();
	Events::EventsManager::GetInstance()->Trigger("INDICES_COUNT", new Events::AnyType<int>(indicesCount));
}

void RenderSystem::CameraActiveHandler(Events::Event* event) {
	const auto camera = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (camera->IsActive()) {
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= camera->depth) {
				cameras.insert(cameras.begin() + i, camera);
				return;
			}
		}
		cameras.push_back(camera);
	} else {
		cameras.erase(vfind(cameras, camera));
	}
}

void RenderSystem::CameraDepthHandler(Events::Event* event) {
	const auto camera = static_cast<Events::AnyType<Camera*>*>(event)->data;

	if (camera->IsActive()) {
		cameras.erase(vfind(cameras, camera));
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= camera->depth) {
				cameras.insert(cameras.begin() + i, camera);
				return;
			}
		}
		cameras.push_back(camera);
	}
}

void RenderSystem::LightActiveHandler(Events::Event* event) {
	const auto light = static_cast<Events::AnyType<Light*>*>(event)->data;

	if (light->IsActive()) {
		lights.push_back(light);
	} else {
		lights.erase(vfind(lights, light));
	}
}

void RenderSystem::RenderActiveHandler(Events::Event* event) { 
	const auto component = static_cast<Events::AnyType<Render*>*>(event)->data;

	if (component->IsActive()) {
		components.push_back(component);
	} else {
		components.erase(vfind(components, component));
	}
}

void RenderSystem::ResizeHandle(Events::Event* event) {
	const auto size = static_cast<Events::AnyType<vec2i>*>(event)->data;
	//for (unsigned i = 0; i < MAX_LIGHTS; ++i) {
	//	depthFBO[i]->Resize(size);
	//}
	windowSize = size;

	const float ratio = static_cast<float>(size.w) / static_cast<float>(size.h);
	const float screen = canvas->size - 1.f;
	canvasLookAt = Math::LookAt(
		vec3f(screen * ratio, -screen, 1.f), 
		vec3f(screen * ratio, -screen, 0.f), 
		vec3f(0.f, 1.f, 0.f)
	);

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

void RenderSystem::Batch() {
	batches.clear();
	const auto cam = cameras[0];
	const auto camEntity = cam->GetParent();
	const auto camPosition = camEntity->GetComponent<Transform>()->GetWorldTranslation();
	const auto camDirection = camEntity->GetComponent<Transform>()->GetLocalFront();
	const auto theta = cos(Math::Rad(cam->FOV));

	auto quad = camEntity->GetQuad();
	while (quad->parent) {
		quad = quad->parent;
	}

	//Traverse(quad, camPosition, camDirection, theta);

	for (auto& list : quad->list) {
		auto c = list->GetComponent<Render>();

		if (!c) continue;

		auto transform = c->GetParent()->GetComponent<Transform>();
		
		if (frustrumCull) {
			auto point = transform->GetWorldTranslation();
			const auto radius = Math::Max(Math::Max(transform->scale.x, transform->scale.y), transform->scale.z);
			point += camDirection * radius;
			const auto dir = Math::Normalized(point - camPosition);

			if (Math::Dot(dir, camDirection) < theta) continue;
		}

		const auto matrix = transform->GetWorldTransform();
		for (auto& mesh : c->model->meshes) {
			batches[c->material->GetShader()][c->material][mesh].push_back(matrix);
		}	
	}

	//for (auto& c : components) {
	//	auto transform = c->GetParent()->GetComponent<Transform>();
	//	
	//	if (frustrumCull) {
	//		auto point = transform->GetWorldTranslation();
	//		const auto radius = Math::Max(Math::Max(transform->scale.x, transform->scale.y), transform->scale.z);
	//		point += camDirection * radius;
	//		const auto dir = Math::Normalized(point - camPosition);

	//		if (Math::Dot(dir, camDirection) < theta) continue;
	//	}

	//	const auto matrix = transform->GetWorldTransform();
	//	for (auto& mesh : c->model->meshes) {
	//		batches[c->material->GetShader()][c->material][mesh].push_back(matrix);
	//		
	//	}
	//}
}

void RenderSystem::Traverse(Quad<Entity*>* const quad, const vec3f & pos, const vec3f & dir, const float & theta) {
	if (!quad->topLeft) {
		if (frustrumCull) {
			auto point = quad->position;
			const auto radius = Math::Max(quad->size.x, quad->size.z);
			point += dir * radius;

			if (Math::Dot(Math::Normalized(point - pos), dir) < theta) return;
		}

		for (auto& list : quad->list) {
			auto c = list->GetComponent<Render>();

			if (!c) continue;

			auto transform = c->GetParent()->GetComponent<Transform>();

			const auto matrix = transform->GetWorldTransform();
			for (auto& mesh : c->model->meshes) {
				batches[c->material->GetShader()][c->material][mesh].push_back(matrix);
			}
		}
	} else {
		Traverse(quad->topLeft, pos, dir, theta);
		Traverse(quad->topRight, pos, dir, theta);
		Traverse(quad->bottomLeft, pos, dir, theta);
		Traverse(quad->bottomRight, pos, dir, theta);
	}
}

void RenderSystem::RenderWorld() {

}

void RenderSystem::SetLightUniforms(Camera* const camera, Shader * const shader) {
	const unsigned count = Math::Min(static_cast<unsigned>(lights.size()), MAX_LIGHTS);

	shader->SetInt("lightCount", count);
	shader->SetVector3("viewPosition", camera->GetParent()->GetComponent<Transform>()->translation);

	for (unsigned i = 0; i < count; ++i) {
		const auto& light = lights[i];
		const auto& transform = light->GetParent()->GetComponent<Transform>();

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