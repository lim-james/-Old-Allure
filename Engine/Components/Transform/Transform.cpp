#include "Transform.h"

#include <Math/MatrixTransform.hpp>

Transform::Transform() 
	: translation(0.0f)
	, scale(1.0f)
	, rotation(0.0f)

	, lockTranslation(false)
	, lockScale(false)
	, lockRotation(false) {}

Transform::~Transform() {}

void Transform::Initialize() {
	Component::Initialize();

	translation.Set(0.0f);
	scale.Set(1.0f);
	rotation.Set(0.0f);

	lockTranslation.Set(false);
	lockScale.Set(false);
	lockRotation.Set(false);
}

vec3f Transform::GetLocalUp() const {
	mat4f transform;
	Math::SetToRotation(transform, vec3f(rotation.x, 0.0f, rotation.z));
	return transform * vec3f(0.0f, 1.0f, 0.0f);
}

vec3f Transform::GetLocalFront() const {
	mat4f transform;
  	Math::SetToRotation(transform, vec3f(rotation.x, -rotation.y, 0.0f));
	return transform * vec3f(0.0f, 0.0f, 1.0f);
}

vec3f Transform::GetLocalRight() const {
	mat4f transform;
  	Math::SetToRotation(transform, vec3f(0.0f, -rotation.y, rotation.z));
	return transform * vec3f(-1.0f, 0.0f, 0.0f);
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