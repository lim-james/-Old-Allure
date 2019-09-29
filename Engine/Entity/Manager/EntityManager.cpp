#include "EntityManager.h"

#include "../../Events/Manager/EventsManager.h"

EntityManager::EntityManager(int start, const unsigned& expand) : EXPAND_SIZE(expand) {
	while (--start >= 0) {
		Entity* e = new Entity;
		pool.insert(e);
		unused.insert(e);
	}

	Events::EventsManager::GetInstance()->Subscribe("ENTITY_USED", &EntityManager::OnUsed, this);
	Events::EventsManager::GetInstance()->Subscribe("ENTITY_DESTROY", &EntityManager::OnDestroy, this);
}

EntityManager::~EntityManager() {
	for (const auto& e : pool)
		delete e;

	pool.clear();
	unused.clear();
}

void EntityManager::Initialize() {
	for (const auto& e : pool)
		e->Initialize();
}

Entity* EntityManager::Fetch() {
	if (unused.empty())
		Expand();

	return *unused.begin();
}

void EntityManager::Expand() {
	for (unsigned i = 0; i < EXPAND_SIZE; ++i) {
		Entity* e = new Entity;
		pool.insert(e);
		unused.insert(e);
	}
}
	
void EntityManager::OnUsed(Events::Event* event) {
	const auto entity = static_cast<Events::AnyType<Entity*>*>(event);
 	unused.erase(entity->data);
}

void EntityManager::OnDestroy(Events::Event* event) {
	const auto entity = static_cast<Events::AnyType<Entity*>*>(event);
	unused.insert(entity->data);
}