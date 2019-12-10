#include "FlyingCamera.h"

#include <Components/Transform/Transform.h>
#include <Components/Camera/Camera.h>
#include <Script/Script.h>

#include <Events/EventsManager.h>
#include <Script/Script.h>
#include <Logger/Logger.h>

#include <GLFW/glfw3.h>

FlyingCamera::FlyingCamera() {
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &FlyingCamera::KeyHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &FlyingCamera::CursorPositionHandler, this);
}

FlyingCamera::~FlyingCamera() {
	keyInputs.clear();
}

void FlyingCamera::Build() {
	CameraObject::Build();
	AddComponent<Script>();
}

void FlyingCamera::Initialize() {
	Entity::Initialize();
	GetComponent<Script>()->update = std::bind(&FlyingCamera::Update, this, std::placeholders::_1);
}

void FlyingCamera::Update(const float& dt) {
	auto transform = GetComponent<Transform>();

	const auto front = transform->GetLocalFront();
	const auto right = transform->GetLocalRight();
	const auto up    = transform->GetLocalUp();

	vec3f dir(0.0f);

	if (keyInputs[GLFW_KEY_W] != GLFW_RELEASE)
		dir += front;

	if (keyInputs[GLFW_KEY_S] != GLFW_RELEASE)
		dir -= front;

	if (keyInputs[GLFW_KEY_A] != GLFW_RELEASE)
		dir -= right;

	if (keyInputs[GLFW_KEY_D] != GLFW_RELEASE)
		dir += right;

	if (keyInputs[GLFW_KEY_SPACE] != GLFW_RELEASE)
		dir += up;

	transform->translation += dir * 10.f * dt;
}

void FlyingCamera::KeyHandler(Events::Event* event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	keyInputs[input->key] = input->action;
}

void FlyingCamera::CursorPositionHandler(Events::Event* event) {
	if (!IsUsed()) return;

	Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);

	auto transform = GetComponent<Transform>(); 
	transform->rotation.y += input->offset.x;
	transform->rotation.x -= input->offset.y;
	transform->rotation.x = Math::Clamp(transform->rotation.x, -89.0f, 89.0f);

	transform->UpdateLocalAxes();
}
