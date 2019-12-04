#ifndef STATE_BASE_H
#define STATE_BASE_H

#include <string>

class Entity;

namespace States {

	class StateBase {

	public:

		virtual void Enter(Entity* const target) = 0;
		virtual void Update(Entity* const target, const float& dt) = 0;
		virtual void FixedUpdate(Entity* const target, const float& dt) = 0;
		virtual void Exit(Entity* const target) = 0;

	};

}


#endif
