#include "EntityManager.h"

#include "../Events/EventsManager.h"

EntityManager::EntityManager(ComponentsManager* manager) 
	: componentsManager(manager)
	, tree(5) {
	Events::EventsManager::GetInstance()->Subscribe("ENTITY_USED", &EntityManager::OnUsed, this);
	Events::EventsManager::GetInstance()->Subscribe("ENTITY_DESTROY", &EntityManager::OnDestroy, this);
}

EntityManager::~EntityManager() {
	typeMap.clear();
	expandSizes.clear();

	for (auto& set : pools) {
		for (auto& c : set.second) {
			delete c;
		}
		set.second.clear();
	}

	pools.clear();
	unused.clear();
}

void EntityManager::Initialize() {
	for (const auto& set : pools) {
		for (const auto& c : set.second) {
			c->Initialize();
		}
	}
}

void EntityManager::Update() {
	tree.root->size.Set(100, 1, 100);
	tree.root->position.Set(0, 0, 0);
	tree.root->list = allObjects;
	tree.Sort();
	
	return;
}


void EntityManager::AddEntity(const unsigned& hash, Entity* entity) {
	entity->componentsManager = componentsManager;
	entity->Build();

	typeMap[entity] = hash;

	allObjects.push_back(entity);

	pools[hash].push_back(entity);
	unused[hash].push_back(entity);
}

void EntityManager::OnUsed(Events::Event* event) {
	const auto& entity = static_cast<Events::AnyType<Entity*>*>(event)->data;
	auto& group = typeMap[entity];

 	unused[group].erase(vfind(unused[group], entity));
	// Adding entity to tree
	used[group].push_back(entity);
}

void EntityManager::OnDestroy(Events::Event* event) {
	const auto& entity = static_cast<Events::AnyType<Entity*>*>(event)->data;
	auto& group = typeMap[entity];
	
	unused[group].push_back(entity);
	// Removing entity from tree
	used[group].erase(vfind(used[group], entity));
}