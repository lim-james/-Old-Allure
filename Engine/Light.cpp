#include "Light.h"

#include <Math/Math.hpp>
#include <Math/MatrixTransform.hpp>
#include <Events/EventsManager.h>

Light::Light()
	: type(POINT)
	
	, ambient(0.1f)
	, diffuse(1.0f)
	, specular(0.5f)

	, power(1.0f) 

	, constant(1.0f)
	, linear(0.09f) 
	, quadratic(0.32f)
	
	, cutOffAngle(12.0f)
	, outerCutOffAngle(24.0f)

	, cutOff(cos(Math::Rad(12.0f)))
	, outerCutOff(cos(Math::Rad(24.0f))) {}

Light::~Light() {}

void Light::Initialize() {
	type = POINT;

	ambient.Set(0.1f);
	diffuse.Set(1.0f);
	specular.Set(0.5f);

	power = 1.0f;

	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.32f;

	SetCutOffAngle(22.5f);
	SetOuterCutOffAngle(45.0f);
}

void Light::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("LIGHT_ACTIVE", new Events::AnyType<Light*>(this));
}

mat4f Light::GetProjectionMatrix() const {
	mat4f result;
	if (type == SPOT) {
		result = Math::Perspective(outerCutOffAngle * 2.0f, 1.0f, 0.1f, 100.0f);
	} else if (type == DIRECTIONAL) {
		result = Math::Orthographic(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
	}

	return result;
}

void Light::SetCutOffAngle(const float& angle) {
	cutOffAngle = angle;
	cutOff = cos(Math::Rad(angle));
}

const float& Light::GetCutOff() const {
	return cutOff;
}

void Light::SetOuterCutOffAngle(const float& angle) {
	outerCutOffAngle = angle;
	outerCutOff = cos(Math::Rad(angle));
}

const float& Light::GetOuterCutOff() const {
	return outerCutOff;
}
