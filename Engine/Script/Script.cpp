#include "Script.h"

#include "../Events/Manager/EventsManager.h"

Script::Script() {
	awake = reset = start = destroy = []() {};
	fixedUpdate = update = [](const float&) {};
}

Script::~Script() {

}

void Script::Initialize() {
	Component::Initialize();

	awake = reset = start = destroy = []() {};
	fixedUpdate = update = [](const float&){};
}

void Script::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("SCRIPT_ACTIVE", new Events::AnyType<Script*>(this));
}

