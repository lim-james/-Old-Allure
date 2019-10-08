#include "Transform.h"

#include <Math/Math.hpp>
#include <Math/MatrixTransform.hpp>

Transform::Transform() 
	: translation(0.0f)
	, scale(1.0f)
	, rotation(0.0f)

	, lockTranslation(false)
	, lockScale(false)
	, lockRotation(false) {
	Update();
}

Transform::~Transform() {}

void Transform::Initialize() {
	Component::Initialize();

	translation.Set(0.0f);
	scale.Set(1.0f);
	rotation.Set(0.0f);

	lockTranslation.Set(false);
	lockScale.Set(false);
	lockRotation.Set(false);

	Update();
}

void Transform::Update() {
	const float yawRad = Math::Rad(rotation.y);
	const float pitchRad = Math::Rad(rotation.x);

	axes.z.x = cos(yawRad) * cos(pitchRad);
	axes.z.y = sin(pitchRad);
	axes.z.z = sin(yawRad) * cos(pitchRad);

	axes.x = Math::Normalized(Math::Cross(axes.z, vec3f(0.0f, 1.0f, 0.0f)));
	axes.y = Math::Normalized(Math::Cross(axes.x, axes.z));
}

const vec3f& Transform::GetLocalUp() const {
	return axes.y;
}

const vec3f& Transform::GetLocalFront() const {
	return axes.z;
}

const vec3f& Transform::GetLocalRight() const {
	return axes.x;
}

mat4f Transform::GetLocalTransform() const {
	mat4f result;
	Math::SetToTransform(result, translation, rotation, scale);
	return result;
}

mat4f Transform::GetLocalLookAt() const {
	const vec3f target = translation + GetLocalFront();
	const vec3f up = GetLocalUp();

	return Math::LookAt(translation, target, up);
}