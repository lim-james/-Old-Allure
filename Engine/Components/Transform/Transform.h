#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../Base/BaseComponent.h"

#include <Math/Vectors.hpp>

struct Transform : public BaseComponent {

	vec3f translation;
	vec3f scale;
	vec3f rotation;

	Transform();
	~Transform();

};

#endif