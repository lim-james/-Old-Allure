#include "ScriptSystem.h"

#include "../Events/EventsManager.h"

#include <MACROS.h>

ScriptSystem::ScriptSystem() {
	Events::EventsManager::GetInstance()->Subscribe("SCRIPT_ACTIVE", &ScriptSystem::ActiveHandler, this);
}

ScriptSystem::~ScriptSystem() {
	components.clear();
}

void ScriptSystem::Update(const float& t) {
	for (auto& c : components) {
		c->update(t);
	}
}

void ScriptSystem::ActiveHandler(Events::Event* event) {
	const auto component = static_cast<Events::AnyType<Script*>*>(event)->data;

	if (component->IsActive()) {
		components.push_back(component);

		component->awake();
		component->reset();
		component->start();
	} else {
		component->destroy();
		components.erase(vfind(components, component));
	}
}