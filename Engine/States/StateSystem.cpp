#include "StateSystem.h"

#include <Events/EventsManager.h>
#include <MACROS.h>

StateSystem::~StateSystem() {
	for (auto& state : states)
		delete state.second;

	states.clear();
}

void StateSystem::Start() {
	Events::EventsManager::GetInstance()->Subscribe("STATE_CONTAINER_ACTIVE", &StateSystem::ActiveHandler, this);
}

void StateSystem::Update(const float& dt) {
	for (auto& c : components) {
		auto entity = c->GetParent();
		if (c->current != "")
			states[c->current]->Update(entity, dt);
	}
}

void StateSystem::FixedUpdate(const float& dt) {
	for (auto& c : components) {
		auto entity = c->GetParent();

		if (c->queued != "") {
			const auto previous = c->current;
			c->current = c->queued;
			if (previous != "")
				states[previous]->Exit(entity);
			c->queued = "";
			states[c->current]->Enter(entity);
		}

		if (c->current != "")
			states[c->current]->FixedUpdate(entity, dt);
	}
}

void StateSystem::ActiveHandler(Events::Event* event) {
	const auto c = static_cast<Events::AnyType<State*>*>(event)->data;

	if (c->IsActive())
		components.push_back(c);
	else
		components.erase(vfind(components, c));

}
