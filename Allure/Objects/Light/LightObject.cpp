#include "LightObject.h"

#include <Components/Transform/Transform.h>
#include <Render/Render.h>
#include <Render/Light/Light.h>
#include <Script/Script.h>

#include <Math/Random.hpp>

void LightObject::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Light>();
	//AddComponent<Script>();
}

void LightObject::Initialize() {
	Entity::Initialize();
	//GetComponent<Script>()->update = std::bind(&LightObject::Update, this, std::placeholders::_1);

	dir = 1.0f;
	et = 0.0f;
}


void LightObject::Update(const float& dt) {
	et += dt;
	auto transform = GetComponent<Transform>();

	const float angle = et * 0.5f;

	transform->rotation.z = 180.f + angle;
	transform->rotation.x = -90.f + angle;

	transform->translation.y = 1.f * abs(sin(angle)) + 5.f;
	transform->translation.x = -cos(angle);
	transform->translation.z = cos(angle);

	//if (transform->translation.x > 2.0f && dir == 1.0f)
	//	dir = -1.0f;
	//else if (transform->translation.x < -2.0f && dir == -1.0f)
	//	dir = 1.0f;

	//transform->translation.x += Math::RandMinMax(1.0f, 2.0f) * dir * dt;

	transform->UpdateLocalAxes();
}
