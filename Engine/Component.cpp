#include "Component.h"

Component::Component()
	: entity(0)
	, active(true) {}

Component::~Component() {}

const bool & Component::IsActive() const {
	return active;
}

void Component::SetActive(const bool & _active) {
	active = _active;
}
