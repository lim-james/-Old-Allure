#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "../Entity.h"

#include "../../Events/Event.h"

#include <vector>

class EntityManager {

	std::map<Entity*, unsigned> typeMap;
	std::map<unsigned, unsigned> expandSizes;

	std::map<std::type_index, std::vector<Entity*>> pools;
	std::map<unsigned, std::vector<Entity*>> unused;

public:

	EntityManager();
	~EntityManager();

	void Initialize();

	template<typename EntityType>
	void Initialize();

	template<typename EntityType>
	const bool Has() const;

	template<typename EntityType>
	void Add(int start, const unsigned& expand);

	template<typename EntityType>
	EntityType* const Fetch();

private:

	template<typename ComponentType>
	void Expand();

	void OnUsed(Events::Event* event);
	void OnDestroy(Events::Event* event);

};


template<typename EntityType>
void EntityManager::Initialize() {
	for (const auto& c : pools[indexof(EntityType)])
		c->Initialize();
}

template<typename EntityType>
const bool EntityManager::Has() const {
	return pools.find(indexof(EntityType)) != pools.end();
}

template<typename EntityType>
void EntityManager::Add(int start, const unsigned& expand) {
	if (Has<EntityType>()) return;

	const auto index = indexof(EntityType);
	const auto hash = hashof(EntityType);

	expandSizes[hash] = expand;

	while (--start >= 0) {
		Entity* entity = new EntityType;
		typeMap[entity] = hash;
		pools[index].push_back(entity);
		unused[hash].push_back(entity);
	}
}

template<typename EntityType>
EntityType* const EntityManager::Fetch() {
	const auto hash = hashof(EntityType);
	if (unused[hash].empty())
		Expand<EntityType>();

	return dynamic_cast<EntityType * const>(*unused[hash].begin());
}

template<typename EntityType>
void EntityManager::Expand() {
	const auto index = indexof(EntityType);
	const auto hash = hashof(EntityType);

	for (unsigned i = 0; i < expandSizes[hash]; ++i) {
		Entity* entity = new EntityType;
		typeMap[entity] = hash;
		pools[index].push_back(entity);
		unused[hash].push_back(entity);
	}
}


#endif