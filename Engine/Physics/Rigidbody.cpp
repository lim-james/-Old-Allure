#include "Rigidbody.h"
<<<<<<< Updated upstream

#include "../Events/EventsManager.h"

Rigidbody::Rigidbody()
{
}


Rigidbody::~Rigidbody()
{
}

void Rigidbody::Initialize()
{
	Component::Initialize();
}

void Rigidbody::SetActive(const bool& state)
{
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("RIGIDBODY_ACTIVE", new Events::AnyType<Rigidbody*>(this));
}
=======
>>>>>>> Stashed changes
