#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Component.h"

#include <Containers/QuadTree.hpp>
#include <Events/Event.h>

#include <MACROS.h>
#include <Logger/Logger.h>

#include <typeindex>
#include <vector>
#include <map>

class EntityManager {

	std::vector<std::map<unsigned, Component*>> entities;

	std::vector<unsigned> used;
	std::vector<unsigned> unused;

public:

	~EntityManager();

	unsigned Create();

	void Destroy(const unsigned& id);

	template<typename ComponentType>
	ComponentType* AddComponent(const unsigned& id);

	template<typename ComponentType>
	bool HasComponent(const unsigned& id);

	template<typename ComponentType>
	ComponentType* GetComponent(const unsigned& id);

	unsigned PoolCount() {
		return entities.size();
	}

private:

	void Expand();

};

template<typename ComponentType>
ComponentType * EntityManager::AddComponent(const unsigned & id) {
	if (id >= entities.size())
		return nullptr;

	const unsigned hash = hashof(ComponentType);

	if (HasComponent<ComponentType>(id))
		return static_cast<ComponentType*>(entities[id][hash]);

	ComponentType* c = new ComponentType;
	c->entity = id;
	c->Initialize();
	entities[id][hash] = c;

	return c;
}

template<typename ComponentType>
bool EntityManager::HasComponent(const unsigned & id) {
	if (id >= entities.size())
		return false;

	return entities[id][hashof(ComponentType)] != nullptr;
}

template<typename ComponentType>
ComponentType* EntityManager::GetComponent(const unsigned & id) {
	if (id >= entities.size())
		return nullptr;

	Component* const c = entities[id][hashof(ComponentType)];
	return c ? static_cast<ComponentType*>(c) : nullptr;
}

#endif

