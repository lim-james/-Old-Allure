#include "Component.h"

#include "../Events/EventsManager.h"

Component::Component() 
	: parent(nullptr)
	, active(false) {
}

Component::~Component() {

}

void Component::Initialize() {
	active = false;
}

void Component::SetActive(const bool& state) {
	active = state;
	Events::EventsManager::GetInstance()->Trigger("COMPONENT_ACTIVE", new Events::AnyType<Component*>(this));
}

const bool& Component::IsActive() const {
	return active;
}