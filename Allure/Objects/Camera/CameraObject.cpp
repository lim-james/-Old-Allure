#include "CameraObject.h"

#include <Components/Transform/Transform.h>

#include <Events/Manager/EventsManager.h>
#include <Script/Script.h>
#include <Logger/Logger.h>

#include <GLFW/glfw3.h>

CameraObject::CameraObject() {
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &CameraObject::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &CameraObject::CursorPositionHandler, this);
}

void CameraObject::Initialize() {
	Entity::Initialize();
	GetComponent<Script>()->update = std::bind(&CameraObject::Update, this, std::placeholders::_1);
}

void CameraObject::Update(const float& dt) {
	for (const auto& key : keyInputs) {
		if (key.action != GLFW_RELEASE) {
			auto transform = GetComponent<Transform>();

			const auto front = transform->GetLocalFront();
			const auto right = transform->GetLocalRight();

			vec3f dir(0.0f);

			if (key.key == GLFW_KEY_W)
				dir += front;
			
			if (key.key == GLFW_KEY_S)
				dir -= front;

			if (key.key == GLFW_KEY_A)
				dir -= right;

			if (key.key == GLFW_KEY_D)
				dir += right;
			
			transform->translation += dir * 10.f * dt;
		}
	}

	keyInputs.clear();
}

void CameraObject::KeyHandler(Events::Event* event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	keyInputs.push_back(*input);
}

void CameraObject::CursorPositionHandler(Events::Event* event) {
	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	GetComponent<Transform>()->rotation.x -= input->offset.y;
	GetComponent<Transform>()->rotation.y -= input->offset.x;
}