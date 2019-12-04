#ifndef STATE_H
#define STATE_H

#include "../Components/Component.h"

#include <string>

struct State : Component {

	std::string queued;
	std::string current;

	State();

	void Initialize() override;
	void SetActive(const bool& state) override;


};

#endif
