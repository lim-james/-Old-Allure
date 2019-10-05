#ifndef CAMERA_H
#define CAMERA_H

#include "../Component.h"

#include "../../Events/Event.h"

#include <Math/Vectors.hpp>
#include <Math/Matrix.hpp>

enum PROJECTION {
	PERSPECTIVE,
	ORTHOGRAPHIC
};

struct Camera : Component {

	vec4f clearColor;

	PROJECTION projection;

	float FOV;

	float nearPlane;
	float farPlane;
	vec4f viewportRect;

	Camera();
	~Camera();

	void Initialize() override;

	void SetActive(const bool& state) override;

	void SetDepth(const float& value);

	mat4f GetProjectionMatrix() const;
	const vec4f& GetViewport() const;

private:

	float depth;

	float aspectRatio;
	float left, right, bottom, top;
	vec4f viewport;

	void WindowResizeHandler(Events::Event* event);

	friend class RenderSystem;

};


#endif