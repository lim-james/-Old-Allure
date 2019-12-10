#include "EntityManager.h"

#include "EntityEvents.h"

// <<<<<<< HEAD
// #include <Events/EventsManager.h>
//
// EntityManager::EntityManager(ComponentsManager* manager)
// 	: componentsManager(manager) {}
// =======
// EntityManager::EntityManager(ComponentsManager* manager)
// 	: componentsManager(manager)
// 	, tree(5) {
// 	Events::EventsManager::GetInstance()->Subscribe("ENTITY_USED", &EntityManager::OnUsed, this);
// 	Events::EventsManager::GetInstance()->Subscribe("ENTITY_DESTROY", &EntityManager::OnDestroy, this);
// }
// >>>>>>> SpatialPartitioning

EntityManager::~EntityManager() {
	typeMap.clear();
	expandSizes.clear();

	for (auto& typeSets : used) {
		for (auto& tagSets : typeSets.second) {
			for (auto& entity : tagSets.second) {
				delete entity;
			}
		}
	}
	used.clear();

	for (auto& typeSets : unused) {
		for (auto& entity : typeSets.second) {
			delete entity;
		}
	}
	unused.clear();
}

void EntityManager::Start() {
	Events::EventsManager::GetInstance()->Subscribe("CREATE_ENTITY", &EntityManager::OnCreate, this);
	Events::EventsManager::GetInstance()->Subscribe("ENTITY_USED", &EntityManager::OnUsed, this);
	Events::EventsManager::GetInstance()->Subscribe("ENTITY_DESTROY", &EntityManager::OnDestroy, this);
	Events::EventsManager::GetInstance()->Subscribe("TAG_CHANGE", &EntityManager::TagChangeHandler, this);
}

void EntityManager::Stop() {
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
}

void EntityManager::Initialize() {
	for (auto& typeSets : used) {
		for (auto& tagSets : typeSets.second) {
			for (auto& entity : tagSets.second) {
				entity->Initialize();
			}
		}
	}

	for (auto& typeSets : unused) {
		for (auto& entity : typeSets.second) {
			entity->Initialize();
		}
	}
}

// <<<<<<< HEAD
// void EntityManager::Destroy() {
// 	for (auto& typeSets : used) {
// 		for (auto& tagSets : typeSets.second) {
// 			auto& list = tagSets.second;
// 			for (int i = static_cast<int>(list.size()) - 1; i >= 0; --i)
// 				list[i]->Destroy();
// 		}
// 	}
// }
//
// const std::map<unsigned, std::vector<Entity*>>& EntityManager::GetEntitiesWithTag(const std::string& tag) {
// 	return used[tag];
// }
// =======
// void EntityManager::Update() {
// 	tree.root->size.Set(100, 1, 100);
// 	tree.root->position.Set(0, 0, 0);
// 	tree.root->list = allObjects;
// 	tree.Sort();
//
// 	return;
// }
//
// >>>>>>> SpatialPartitioning

void EntityManager::AddEntity(const unsigned& hash, Entity* entity) {
	entity->componentsManager = componentsManager;
	entity->Build();

	typeMap[entity] = hash;
// <<<<<<< HEAD
// 	unused[hash].push_back(entity);
// }
//
// void EntityManager::OnCreate(Events::Event* event) {
// 	Events::CreateEntity* entityEvent = static_cast<Events::CreateEntity*>(event);
// 	auto entity = entityEvent->entity;
// 	AddEntity(entityEvent->hash, entity);
// 	entity->Initialize();
// 	entity->Use();
// =======
//
// 	allObjects.push_back(entity);
//
// 	pools[hash].push_back(entity);
// 	unused[hash].push_back(entity);
// >>>>>>> SpatialPartitioning
}

void EntityManager::OnUsed(Events::Event* event) {
	const auto& entity = static_cast<Events::AnyType<Entity*>*>(event)->data;
// <<<<<<< HEAD
// 	const auto hash = typeMap[entity];
// 	auto& unusedGroup = unused[hash];
//
// 	used[entity->GetTag()][hash].push_back(entity);
// 	unusedGroup.erase(vfind(unusedGroup, entity));
// =======
// 	auto& group = typeMap[entity];
//
//  	unused[group].erase(vfind(unused[group], entity));
// 	// Adding entity to tree
// 	used[group].push_back(entity);
// >>>>>>> SpatialPartitioning
}

void EntityManager::OnDestroy(Events::Event* event) {
	const auto& entity = static_cast<Events::AnyType<Entity*>*>(event)->data;
// <<<<<<< HEAD
// 	const auto hash = typeMap[entity];
// 	auto& usedGroup = used[entity->GetTag()][hash];
//
// 	usedGroup.erase(vfind(usedGroup, entity));
// 	unused[hash].push_back(entity);
// }
//
// void EntityManager::TagChangeHandler(Events::Event* event) {
// 	auto changeEvent = static_cast<Events::TagChange*>(event);
// 	auto entity = changeEvent->entity;
// 	const auto hash = typeMap[entity];
//
// 	auto& previous = used[changeEvent->previous][hash];
// 	previous.erase(vfind(previous, entity));
// 	used[entity->GetTag()][hash].push_back(entity);
// }
// =======
// 	auto& group = typeMap[entity];
//
// 	unused[group].push_back(entity);
// 	// Removing entity from tree
// 	used[group].erase(vfind(used[group], entity));
// }
// >>>>>>> SpatialPartitioning
