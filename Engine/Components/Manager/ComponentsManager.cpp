#include "ComponentsManager.h"

#include "../../Events/Manager/EventsManager.h"

ComponentsManager::ComponentsManager(const unsigned& start, const unsigned& expand) 
	: START_SIZE(start)
	, EXPAND_SIZE(expand) {
	Events::EventsManager::GetInstance()->Subscribe("COMPONENT_ACTIVE", &ComponentsManager::ActiveHandle, this);
}

ComponentsManager::~ComponentsManager() {
	typeMap.clear();

	for (auto& set: components) {
		for (auto& c : set.second) {
			delete c;
		}
		set.second.clear();
	}
	components.clear();
	unused.clear();
}

void ComponentsManager::Initialize() {
	for (const auto& set : components) {
		for (const auto& c : set.second) {
			c->Initialize();
		}
	}
}

void ComponentsManager::ActiveHandle(Events::Event* event) {
	const auto component = static_cast<Events::AnyType<Component*>*>(event);
	const auto hash = typeMap[component->data];

	if (component->data->IsActive()) {
		unused[hash].erase(component->data);
	} else {
		unused[hash].insert(component->data);
	}
}