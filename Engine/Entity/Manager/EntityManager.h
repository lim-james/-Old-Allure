#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "../Entity.h"

#include "../../Events/Event.h"

#include <set>

class EntityManager {

	const unsigned EXPAND_SIZE;
	
	std::set<Entity*> pool;
	std::set<Entity*> unused;

public:

	EntityManager(int start, const unsigned& expand);
	~EntityManager();

	void Initialize();
	
	Entity* Fetch();

private:

	void Expand();

	void OnUsed(Events::Event* event);
	void OnDestroy(Events::Event* event);

};

#endif