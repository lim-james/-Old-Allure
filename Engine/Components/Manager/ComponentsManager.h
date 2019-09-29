#ifndef COMPONENTS_MANAGER_H
#define COMPONENTS_MANAGER_H

#include "../Component.h"
#include "../../Events/Event.h"

#include <MACROS.h>

#include <set>
#include <map>
#include <typeindex>

class ComponentsManager {

	const unsigned START_SIZE;
	const unsigned EXPAND_SIZE;

	std::map<Component*, unsigned> typeMap;
	std::map<std::type_index, std::set<Component*>> components;
	std::map<unsigned, std::set<Component*>> unused;

public:

	ComponentsManager(const unsigned& start, const unsigned& expand);
	~ComponentsManager();

	template<typename ComponentType>
	const bool Has() {
		return components.find(indexof(ComponentType)) != components.end();
	}

	template<typename ComponentType>
	void Add() {
		if (Has<ComponentType>()) return;

		const auto index = indexof(ComponentType);
		const auto hash = hashof(ComponentType);

		for (unsigned i = 0; i < START_SIZE; ++i) {
			ComponentType* component = new ComponentType;
			typeMap[component] = hash;
			components[index].insert(component);
			unused[hash].insert(component);
		}
	}

	void Initialize();

	template<typename ComponentType>
	void Initialize() {
		for (const auto& c : components[indexof(ComponentType)]) {
			c->Initialize();
		}
	}

	template<typename ComponentType>
	ComponentType* const Fetch() {
		auto& set = unused[hashof(ComponentType)];
		if (set.empty())
			Expand<ComponentType>();

		return dynamic_cast<ComponentType* const>(*set.begin());
	}

private:

	template<typename ComponentType>
	void Expand() {
		const auto index = indexof(ComponentType);
		const auto hash = hashof(ComponentType);

		for (unsigned i = 0; i < EXPAND_SIZE; ++i) {
			ComponentType* component = new ComponentType;
			typeMap[component] = hash;
			components[index].insert(component);
			unused[hash].insert(component);
		}
	}

	void ActiveHandle(Events::Event* event);

};

#endif