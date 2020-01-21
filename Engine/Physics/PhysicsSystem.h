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

	bool partition;

	int collisionCount;
	//CollisionData data;

public:
	PhysicsSystem();
	~PhysicsSystem() override;

	void Start();

	void Update(const float& t);
	void FixedUpdate(const float& t);

	void RigidbodyActiveHandler(Events::Event* event);
	void ColliderActiveHandler(Events::Event* event);
	void PartitionHandler(Events::Event* event);
	//void BoxColliderActiveHandler(Events::Event* event);
	void ApplyGravity(const float& t);
	void UpdateVelocity(const float& t);
	bool CollisionCheck(Collider* c1, Collider* c2, float dt);
	void CollisionResponse(Collider* c1, Collider* c2);
	bool SphereToSphereCollision(Collider * c1, Collider * c2, CollisionData * data, float dt);
	bool SphereToWallCollision(Collider* c1, Collider* c2, CollisionData* data);
};

#endif