#include "CameraObject.h"

#include <Components/Transform/Transform.h>
#include <Components/Camera/Camera.h>

void CameraObject::Build() {
	AddComponent<Transform>();
	AddComponent<Camera>();
}