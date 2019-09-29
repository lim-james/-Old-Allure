#include "Transform.h"

Transform::Transform() 
	: translation(0.0f)
	, scale(1.0f)
	, rotation(0.0f) {
}

Transform::~Transform() {

}

void Transform::Initialize() {
	Component::Initialize();

	translation.Set(0.0f);
	scale.Set(1.0f);
	rotation.Set(0.0f);
}