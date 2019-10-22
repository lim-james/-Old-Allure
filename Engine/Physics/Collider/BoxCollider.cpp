#include "BoxCollider.h"

#include "../../Events/EventsManager.h"

BoxCollider::BoxCollider()
	: size(1.f, 1.f, 1.f)
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Initialize()
{
	Component::Initialize();


}

void BoxCollider::SetActive(const bool& state)
{
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("BOXCOLLIDER_ACTIVE", new Events::AnyType<BoxCollider*>(this));

}
