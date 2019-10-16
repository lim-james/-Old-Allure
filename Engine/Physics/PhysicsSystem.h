#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../Systems/System.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	void Update(const float& t) override;
};

#endif