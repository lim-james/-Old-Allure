#ifndef LIGHT_OBJECT_H
#define LIGHT_OBJECT_H

#include <Entity/Entity.h>

class LightObject : public Entity {

	float dir;

public:

	void Build() override;

	void Initialize() override;

private:

	void Update(const float& dt);

};

#endif