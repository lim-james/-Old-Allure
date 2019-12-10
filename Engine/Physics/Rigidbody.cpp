#include "Rigidbody.h"

#include "../Entity/Entity.h"
#include "../Components/Transform/Transform.h"

#include "../Core/Events/EventsManager.h"

Rigidbody::Rigidbody()
	: mass(1.f)
	, hasGravity(true)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Initialize()
{
	Component::Initialize();

}

void Rigidbody::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("RIGIDBODY_ACTIVE", new Events::AnyType<Rigidbody*>(this));
}

void Rigidbody::AddForce(vec3f force) {
	this->GetParent()->GetComponent<Transform>()->translation += force;
}

void Rigidbody::ConstantForce(vec3f force) {
	
}
