#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../Component.h"

#include <Math/Vectors.hpp>
#include <Math/Matrix.hpp>

struct Transform : Component {

	vec3f translation;
	vec3f scale;
	vec3f rotation;

	Math::vec3<bool> lockTranslation;
	Math::vec3<bool> lockScale;
	Math::vec3<bool> lockRotation;

	Math::vec3<vec3f> axes;

	Transform();
	~Transform() override;

	void Initialize() override;
	void Update();

	const vec3f& GetLocalUp() const;
	const vec3f& GetLocalFront() const;
	const vec3f& GetLocalRight() const;

	mat4f GetLocalTransform() const;
	mat4f GetLocalLookAt() const;

};

#endif