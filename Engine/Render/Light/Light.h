#ifndef LIGHT_H
#define LIGHT_H

#include "../../Components/Component.h"

#include <Math/Vectors.hpp>

struct Light : Component {

	enum TYPE {
		POINT,
		DIRECTIONAL,
		SPOT
	} type;

	vec3f ambient;
	vec3f diffuse;
	vec3f specular;

	float power;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

	Light();
	~Light() override;

	void Initialize() override;

	void SetActive(const bool& state) override;

};

#endif