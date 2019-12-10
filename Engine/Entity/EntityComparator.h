#ifndef ENTITY_COMPARATOR_H
#define ENTITY_COMPARATOR_H

#include "Entity.h"

#include <Quad.hpp>


struct EntityComparator : Comparator<Entity*>
{
	EntityComparator();
	~EntityComparator();

	void Partition(Quad<Entity*>* _root) override;
};

#endif // !ENTITY_COMPARATOR_H
