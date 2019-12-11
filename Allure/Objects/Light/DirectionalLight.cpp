#include "DirectionalLight.h"

#include <Components/Transform/Transform.h>
#include <Render/Light/Light.h>
#include <Script/Script.h>

void DirectionalLight::Build() {
	AddComponent<Transform>();
	AddComponent<Light>();
	AddComponent<Script>();
}

void DirectionalLight::Initialize() {
	Entity::Initialize();

	GetComponent<Light>()->type = Light::DIRECTIONAL;
	//GetComponent<Script>()->update = std::bind(&DirectionalLight::Update, this, std::placeholders::_1);

	et = 0.0f;
}


void DirectionalLight::Update(const float& dt) {
	et += dt;
	auto transform = GetComponent<Transform>();

	const float angle = et * 30.f;

	transform->rotation.y = 180 + et * 30.f;
	
	//if (transform->translation.x > 2.0f && dir == 1.0f)
	//	dir = -1.0f;
	//else if (transform->translation.x < -2.0f && dir == -1.0f)
	//	dir = 1.0f;

	transform->translation.x = 10.f * sin(Math::Rad(angle));
	transform->translation.z = 10.f * cos(Math::Rad(angle));

	//transform->translation.x += Math::RandMinMax(1.0f, 2.0f) * dir * dt;

	transform->UpdateLocalAxes();
}
