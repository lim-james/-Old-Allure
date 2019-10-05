#include "ComponentsManager.h"

#include "../../Events/Manager/EventsManager.h"

ComponentsManager::ComponentsManager() {
	Events::EventsManager::GetInstance()->Subscribe("COMPONENT_ACTIVE", &ComponentsManager::ActiveHandle, this);
}

ComponentsManager::~ComponentsManager() {
	typeMap.clear();
	expandSizes.clear();

	for (auto& set: pools) {
		for (auto& c : set.second) {
			delete c;
		}
		set.second.clear();
	}

	pools.clear();
	unused.clear();
}

void ComponentsManager::Initialize() {
	for (const auto& set : pools) {
		for (const auto& c : set.second) {
			c->Initialize();
		}
	}
}

void ComponentsManager::ActiveHandle(Events::Event* event) {
	const auto& component = static_cast<Events::AnyType<Component*>*>(event)->data;
	auto& unusedGroup = unused[typeMap[component]];

	if (component->IsActive()) {
		unusedGroup.erase(vfind(unusedGroup, component));
	} else {
		unusedGroup.push_back(component);
	}
}