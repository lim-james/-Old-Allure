#ifndef LIGHT_H
#define LIGHT_H

#include "../../Components/Component.h"

#include <Math/Vectors.hpp>
#include <Math/Matrix.hpp>

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


	Light();
	~Light() override;

	void Initialize() override;

	void SetActive(const bool& state) override;

	mat4f GetProjectionMatrix() const;

	void SetCutOffAngle(const float& angle);
	const float& GetCutOff() const;

	void SetOuterCutOffAngle(const float& angle);
	const float& GetOuterCutOff() const;

private:

	float cutOffAngle;
	float outerCutOffAngle;

	float cutOff;
	float outerCutOff;

};

#endif