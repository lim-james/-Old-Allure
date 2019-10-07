#include "Light.h"

#include "../../Events/EventsManager.h"

#include <Math/Math.hpp>

Light::Light()
	: type(POINT)
	
	, ambient(0.2f)
	, diffuse(1.0f)
	, specular(0.5f)

	, power(1.0f) 

	, constant(1.0f)
	, linear(0.09f) 
	, quadratic(0.32f)
	
	, cutOff(cos(Math::Rad(12.0f)))
	, outerCutOff(cos(Math::Rad(24.0f))) {}

Light::~Light() {}

void Light::Initialize() {
	Component::Initialize();

	type = POINT;

	ambient.Set(0.2f);
	diffuse.Set(1.0f);
	specular.Set(0.5f);

	power = 1.0f;

	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.32f;

	cutOff = cos(Math::Rad(12.0f));
	outerCutOff = cos(Math::Rad(24.0f));
}

void Light::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("LIGHT_ACTIVE", new Events::AnyType<Light*>(this));
}
