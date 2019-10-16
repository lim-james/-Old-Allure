#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../Systems/System.h"

#include "Rigidbody.h"

#include <vector>

class PhysicsSystem : public System
{
	std::vector<Rigidbody*> rigidbody;

public:
	PhysicsSystem();
	~PhysicsSystem();

	void Update(const float& t) override;

	void SetGravity();
};

#endif