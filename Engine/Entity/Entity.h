#ifndef ENTITY_H
#define ENTITY_H

#include "../Components/Component.h"

#include <MACROS.h>

#include <map>
#include <typeindex>
#include <string>

class Entity {

	bool used;
	bool staticEntity;

	std::map<std::type_index, Component*> components;

public:

	std::string tag;

	Entity();
	~Entity();

	void Initialize();
	void Destroy();

	void SetActive(const bool& state);

	void Use();
	const bool& IsUsed() const;

	template<typename ComponentType>
	const bool HasComponent() const {
		return components.find(indexof(ComponentType)) != components.end();
	}

	template<typename ComponentType>
	void AddComponent(ComponentType * const component) {
		if (HasComponent<ComponentType>()) return;

		component->parent = this;
		components[indexof(ComponentType)] = component;
	}

	template<typename ComponentType>
	ComponentType* const GetComponent() {
		return dynamic_cast<ComponentType* const>(components[indexof(ComponentType)]);
	}

};

#endif