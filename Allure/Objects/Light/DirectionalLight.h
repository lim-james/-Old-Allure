#ifndef DIRECTIONAL_LIGHT_OBJECT_H
#define DIRECTIONAL_LIGHT_OBJECT_H

#include "LightObject.h"

class DirectionalLight : public LightObject {

	float et;

public:

	void Build() override;
	void Initialize() override;

private:

	void Update(const float& dt);


};

#endif
