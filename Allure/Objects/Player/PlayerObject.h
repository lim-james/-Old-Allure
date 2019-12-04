#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <Entity/Entity.h>

class PlayerObject : public Entity {

	int forwardKey, backwardsKey;

	std::map<int, int> keyInputs;

public:

	PlayerObject();

	void Build() override;
	void Initialize() override;

	void SetForwardKey(const int& key);
	void SetBackwardsKey(const int& key);

private:

	void Update(const float& dt);

	void KeyHandler(Events::Event* event);

};

#endif