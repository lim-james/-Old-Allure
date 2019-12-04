#include "PlayerObject.h"

#include "../../Input/InputEvents.h"

#include <Components/Transform/Transform.h>
#include <Render/Render.h>
#include <Script/Script.h>

#include <Events/EventsManager.h>

#include <GLFW/glfw3.h>

PlayerObject::PlayerObject() {
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &PlayerObject::KeyHandler, this);
}


void PlayerObject::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Script>();
}

void PlayerObject::Initialize() {
	Entity::Initialize();
	GetComponent<Script>()->update = std::bind(&PlayerObject::Update, this, std::placeholders::_1);
}

void PlayerObject::SetForwardKey(const int& key) {
	forwardKey = key;
}

void PlayerObject::SetBackwardsKey(const int& key) {
	backwardsKey = key;
}

void PlayerObject::Update(const float& dt) {
	auto transform = GetComponent<Transform>();

	const auto front = transform->GetLocalFront();

	vec3f dir(0.0f);

	// TODO: [Irwen] Implement physics movement
	if (keyInputs[forwardKey] != GLFW_RELEASE)
		dir += front;

	if (keyInputs[backwardsKey] != GLFW_RELEASE)
		dir -= front;

	transform->translation += dir * 10.f * dt;
}

void PlayerObject::KeyHandler(Events::Event* event) {
	Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	keyInputs[input->key] = input->action;
}
