#include "PhysicsSystem.h"

#include "../Entity/Entity.h"
#include "../Components/Transform/Transform.h"

#include "../Events/EventsManager.h"

PhysicsSystem::PhysicsSystem()
{
	Events::EventsManager::GetInstance()->Subscribe("RIGIDBODY_ACTIVE", &PhysicsSystem::PhysicsActiveHandler, this);
}


PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Update(const float & t)
{
	SetGravity(t);
}

void PhysicsSystem::PhysicsActiveHandler(Events::Event* event)
{
	const auto component = static_cast<Events::AnyType<Rigidbody*>*>(event)->data;
	if (component->IsActive())
	{
		rigidbody.push_back(component);
	}

}

void PhysicsSystem::SetGravity(const float& t)
{
	for (auto& r : rigidbody)
	{
		r->GetParent()->GetComponent<Transform>()->translation += vec3f(0, -9.81, 0) * t;
	}
}
