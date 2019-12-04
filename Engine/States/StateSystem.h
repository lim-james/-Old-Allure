#ifndef STATE_SYSTEM_H
#define STATE_SYSTEM_H

#include "../Systems/System.h"
#include "State.h"

#include "StateBase.h"

#include <Events/Event.h>

#include <vector>
#include <map>

class StateSystem : public System {

	std::map<std::string, States::StateBase*> states;
	std::vector<State*> components;

public:

	~StateSystem() override;

	void Start() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

	template<typename State>
	void AttachState(const std::string& key);

private:

	void ActiveHandler(Events::Event* event);

};


template<typename State>
void StateSystem::AttachState(const std::string& key) {
	states[key] = new State;
}

#endif

