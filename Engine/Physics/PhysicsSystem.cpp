#include "PhysicsSystem.h"

<<<<<<< Updated upstream
#include "../Entity/Entity.h"	
#include "../Components/Transform/Transform.h"

#include "../Events/EventsManager.h"

PhysicsSystem::PhysicsSystem()
{
	Events::EventsManager::GetInstance()->Subscribe("RIGIDBODY_ACTIVE", &PhysicsSystem::RigidbodyActiveHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("BOXCOLLIDER_ACTIVE", &PhysicsSystem::BoxColliderActiveHandler, this);
}


=======
PhysicsSystem::PhysicsSystem()
{
}

>>>>>>> Stashed changes
PhysicsSystem::~PhysicsSystem()
{
}

<<<<<<< Updated upstream
void PhysicsSystem::Update(const float & t)
{
	SetGravity(t);
}

void PhysicsSystem::RigidbodyActiveHandler(Events::Event* event)
{
	const auto component = static_cast<Events::AnyType<Rigidbody*>*>(event)->data;
	if (component->IsActive())
	{
		rigidbody.push_back(component);
	}
}

void PhysicsSystem::BoxColliderActiveHandler(Events::Event* event)
{
	const auto component = static_cast<Events::AnyType<BoxCollider*>*>(event)->data;
	if (component->IsActive())
	{
		boxcollider.push_back(component);
	}
}

void PhysicsSystem::SetGravity(const float& t)
{
	for (auto& r : rigidbody)
	{
		r->GetParent()->GetComponent<Transform>()->translation += vec3f(0, -9.81, 0) * t;
	}
=======
void PhysicsSystem::Start()
{
}

void PhysicsSystem::Update(const float& t)
{
}

void PhysicsSystem::FixedUpdate(const float& t)
{
>>>>>>> Stashed changes
}
