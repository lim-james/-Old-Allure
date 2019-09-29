#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../Component.h"

#include <Math/Vectors.hpp>

struct Transform : public Component {

	vec3f translation;
	vec3f scale;
	vec3f rotation;

	Transform();
	~Transform() override;

	void Initialize() override;

};

#endif