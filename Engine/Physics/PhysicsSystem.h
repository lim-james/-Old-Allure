#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../Systems/System.h"

#include "Rigidbody.h"

#include "../Events/Event.h"

#include <vector>

class PhysicsSystem : public System
{
	std::vector<Rigidbody*> rigidbody;

public:
	PhysicsSystem();
	~PhysicsSystem() override;

	void Update(const float& t) override;

	void PhysicsActiveHandler(Events::Event* event);

	void SetGravity(const float& t);
};

#endif