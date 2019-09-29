#include "Entity.h"

Entity::Entity() 
	: staticEntity(false) {
}

Entity::~Entity() {
	components.clear();
}