#include "ScriptSystem.h"

#include <Events/EventsManager.h>

#include <MACROS.h>

ScriptSystem::~ScriptSystem() {
	components.clear();
}

void ScriptSystem::Start() {
	Events::EventsManager::GetInstance()->Subscribe("SCRIPT_ACTIVE", &ScriptSystem::ActiveHandler, this);
}

void ScriptSystem::Initialize() {
	for (auto& c : components) {
		c->reset();
		c->start();
	}
}

void ScriptSystem::Update(const float& dt) {
	for (auto& c : components) 
		c->update(dt);
}

void ScriptSystem::FixedUpdate(const float & dt) {
	for (auto& c : components) 
		c->fixedUpdate(dt);
}

void ScriptSystem::ActiveHandler(Events::Event* event) {
	const auto component = static_cast<Events::AnyType<Script*>*>(event)->data;

	if (component->IsActive()) {
		component->awake();
		components.push_back(component);
	} else {
		component->destroy();
		components.erase(vfind(components, component));
	}
}