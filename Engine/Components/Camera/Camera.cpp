#include "Camera.h"

#include "../../Events/EventsManager.h"

#include <Math/MatrixTransform.hpp>
#include <Logger/Logger.h>

Camera::Camera()
	: clearColor(0.f)

	, projection(PERSPECTIVE)
	, FOV(45.0f)

	, nearPlane(0.01f)
	, farPlane(1000.0f)

	, depth(0.0f) 
	, viewportRect(0.0f, 0.0f, 1.0f, 1.0f) 

	, aspectRatio(1.f) 
	, left(0.f)
	, right(1.f)
	, bottom(0.f)
	, top(1.f)
	, viewport(vec2f(0.f), vec2f(1.f))

	, windowSize(1.f) {

	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &Camera::WindowResizeHandler, this);
}

Camera::~Camera() {}

void Camera::Initialize() {
	Component::Initialize();

	clearColor.Set(0.f);

	projection = PERSPECTIVE;
	FOV = 45.0f;
	
	nearPlane = 0.01f;
	farPlane = 1000.0f;

	depth = 0.0f;
	viewportRect.Set(vec2f(0.f), vec2f(1.f));

	vec2i size;
	Events::EventsManager::GetInstance()->Trigger("GET_WINDOW_SIZE", new Events::AnyType<vec2i*>(&size));
	windowSize = size;

	UpdateViewport();
}

void Camera::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("CAMERA_ACTIVE", new Events::AnyType<Camera*>(this));
}

void Camera::SetDepth(const float& value) {
	depth = value;
	Events::EventsManager::GetInstance()->Trigger("CAMERA_DEPTH", new Events::AnyType<Camera*>(this));
}

mat4f Camera::GetProjectionMatrix() const {
	mat4f result;
	if (projection == PERSPECTIVE) {
		result = Math::Perspective(FOV, aspectRatio, nearPlane, farPlane);
	} else {
		result = Math::Orthographic(left, right, bottom, top, nearPlane, farPlane);
	}

	return result;
}

void Camera::SetViewportRect(const vec4f& rect) {
	viewportRect = rect;
	UpdateViewport();
}

const vec4f& Camera::GetViewport() const {
	return viewport;
}

void Camera::WindowResizeHandler(Events::Event* event) {
	windowSize = static_cast<Events::AnyType<vec2i>*>(event)->data;
	UpdateViewport();
}

void Camera::UpdateViewport() {
	viewport = vec4f(windowSize, windowSize) * viewportRect;
	aspectRatio = viewport.size.w / viewport.size.h;
	left = viewport.origin.x, right = viewport.origin.x + viewport.size.w;
	bottom = viewport.origin.y, top = viewport.origin.y + viewport.size.h;
}