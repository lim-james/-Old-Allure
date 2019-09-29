#ifndef ENTITY_H
#define ENTITY_H

#include "../Components/Base/BaseComponent.h"

#include <MACROS.h>

#include <typeindex>
#include <map>

class Entity {

	bool staticEntity;

	std::map<std::type_index, BaseComponent*> components;

public:

	Entity();
	~Entity();

	template<typename Component>
	const bool HasComponent() const {
		return components.find(indexof(Component)) != components.end();
	}

	template<typename Component>
	void AddComponent() {
		if (HasComponent<Component>()) return;

		Component* component = new Component;
		component->parent = this;
		components[indexof(Component)] = component;
	}

	template<typename Component>
	void AddComponent(Component const * const component) {
		if (HasComponent<Component>()) return;

		component->parent = this;
		components[indexof(Component)] = component;
	}

	template<typename Component>
	Component * const GetComponent() {
		return dynamic_cast<Component*>(components[indexof(Component)]);
	}

};

#endif