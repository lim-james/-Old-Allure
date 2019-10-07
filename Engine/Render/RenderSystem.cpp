#include "RenderSystem.h"

#include "../Entity/Entity.h"
#include "../Components/Transform/Transform.h"

#include "../Events/EventsManager.h"

#include <Logger/Logger.h>
#include <Math/Math.hpp>

#include <GL/glew.h>

#include <algorithm>

RenderSystem::RenderSystem() {
	lights.reserve(MAX_LIGHTS);

	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("LIGHT_ACTIVE", &RenderSystem::LightActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &RenderSystem::RenderActiveHandler, this);

	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
}

RenderSystem::~RenderSystem() {
	cameras.clear();
	lights.clear();
	components.clear();
}

void RenderSystem::Update(const float& t) {
	if (cameras.empty()) return;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& cam : cameras) {
		const auto& viewport = cam->GetViewport();
		const auto& projection = cam->GetProjectionMatrix();
		const auto& lookAt = cam->parent->GetComponent<Transform>()->GetLocalLookAt();
	
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

		for (auto& c : components) {
			if (!c->model) continue;
			
			c->material->Use();

			auto shader = c->material->shader;
			
			shader->SetMatrix4("projection", projection);
			shader->SetMatrix4("view", lookAt);
			shader->SetMatrix4("model",	c->GetParent()->GetComponent<Transform>()->GetLocalTransform());

			if (c->material->lit)
				SetLightUniforms(cam, shader);
			
			for (const auto& mesh : c->model->meshes) {
				glBindVertexArray(mesh->VAO);
				glDrawElements(GL_TRIANGLES, mesh->indicesSize, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}
	}
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

void RenderSystem::SetLightUniforms(Camera* const camera, Shader * const shader) {
	const unsigned count = Math::Min(lights.size(), MAX_LIGHTS);

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

		shader->SetFloat(tag + "cutOff", light->cutOff);
		shader->SetFloat(tag + "outerCutOff", light->outerCutOff);
	}
}