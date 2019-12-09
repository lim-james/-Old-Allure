#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../Systems/System.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem();
	~PhysicsSystem() override;

	void Start();
	void Update(const float& t);
	void FixedUpdate(const float& t);

};

#endif