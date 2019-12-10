#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "../Systems/System.h"


#include "Rigidbody.h"
#include "Collider/Collider.h"

#include <Events/Event.h>

#include <vector>

class PhysicsSystem : public System
{
	std::vector<Rigidbody*> rigidbody;
	std::vector<Collider*> collider;

	vec3f const GRAVITY_FORCE = vec3f(0, -9.81, 0);

	vec3f gravity;

public:
	PhysicsSystem();
	~PhysicsSystem() override;

	void Start();

	void Update(const float& t);
	void FixedUpdate(const float& t);

	void RigidbodyActiveHandler(Events::Event* event);
	void ColliderActiveHandler(Events::Event* event);
	//void BoxColliderActiveHandler(Events::Event* event);
	void UpdateVelocity(const float& t);
	bool CollisionCheck(Collider* c1, Collider* c2);
	void CollisionResponse(Collider* c1, Collider* c2);
	void SphereToSphereCollision(Collider * c1, Collider * c2);
};

#endif