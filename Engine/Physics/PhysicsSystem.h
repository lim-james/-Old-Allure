#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../Systems/System.h"

#include "Rigidbody.h"
#include "Collider/BoxCollider.h"

#include "../Events/Event.h"

#include <vector>

class PhysicsSystem : public System
{

	std::vector<Rigidbody*> rigidbody;
	std::vector<BoxCollider*> boxcollider;

public:

	PhysicsSystem();
	~PhysicsSystem() override;

	void Update(const float& t) override;

	void RigidbodyActiveHandler(Events::Event* event);
	void BoxColliderActiveHandler(Events::Event* event);

	void SetGravity(const float& t);

};

#endif