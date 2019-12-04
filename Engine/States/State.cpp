#include "State.h"

#include <Events/EventsManager.h>

State::State()
	: queued("")
	, current("") {}

void State::Initialize() {
	Component::Initialize();

	queued = "";
	current = "";
}

void State::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("STATE_CONTAINER_ACTIVE", new Events::AnyType<State*>(this));
}
