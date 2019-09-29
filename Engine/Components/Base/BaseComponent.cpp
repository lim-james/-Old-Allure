#include "BaseComponent.h"

BaseComponent::BaseComponent() 
	: parent(nullptr)
	, active(false) {
}

BaseComponent::~BaseComponent() {

}

void BaseComponent::SetActive(const bool& state) {
	active = state;
}

const bool& BaseComponent::IsActive() const {
	return active;
}