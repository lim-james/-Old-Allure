#include "Collider.h"

#include "../Core/Events/EventsManager.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

void Collider::Initialize()
{
	Component::Initialize();
}

void Collider::SetActive(const bool& state)
{
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("COLLIDER_ACTIVE", new Events::AnyType<Collider*>(this));
}

void Collider::OnCollisionEnter(Entity* collided)
{

}

void Collider::OnCollisionStay(Entity* collided)
{
}
