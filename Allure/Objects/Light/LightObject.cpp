#include "LightObject.h"

#include <Components/Transform/Transform.h>
#include <Render/Render.h>
#include <Render/Light/Light.h>

void LightObject::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Light>();
}