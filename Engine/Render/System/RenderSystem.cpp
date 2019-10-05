#include "RenderSystem.h"

#include "../../Entity/Entity.h"
#include "../../Components/Transform/Transform.h"

#include "../../Events/Manager/EventsManager.h"

#include <Logger/Logger.h>

#include <GL/glew.h>

#include <algorithm>

RenderSystem::RenderSystem() {
	shader = new Shader("Files/Shaders/simple.vert", "Files/Shaders/simple.frag");

	Events::EventsManager::GetInstance()->Subscribe("CAMERA_ACTIVE", &RenderSystem::CameraActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CAMERA_DEPTH", &RenderSystem::CameraDepthHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &RenderSystem::RenderActiveHandler, this);

	glEnable(GL_SCISSOR_TEST);
}

RenderSystem::~RenderSystem() {
	delete shader;
	components.clear();
}

void RenderSystem::Update(const float& t) {
	if (cameras.empty()) return;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->Use();

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
		glClear(GL_COLOR_BUFFER_BIT);

		shader->SetMatrix4("projection", projection);
		shader->SetMatrix4("view", lookAt);

		for (auto& c : components) {
			if (!c->model) continue;

			shader->SetMatrix4("model",	c->parent->GetComponent<Transform>()->GetLocalTransform());

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
		Console::Log << "Added a camera component.\n";
		for (unsigned i = 0; i < cameras.size(); ++i) {
			if (cameras[i]->depth >= camera->depth) {
				cameras.insert(cameras.begin() + i, camera);
				return;
			}
		}
		cameras.push_back(camera);
	} else {
		cameras.push_back(camera);
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

void RenderSystem::RenderActiveHandler(Events::Event* event) { 
	const auto component = static_cast<Events::AnyType<Render*>*>(event)->data;

	if (component->IsActive()) {
		components.push_back(component);
		Console::Log << "Added a render component.\n";
	} else {
		components.push_back(component);
	}
}