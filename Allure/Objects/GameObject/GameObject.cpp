#include "GameObject.h"

#include <Components/Transform/Transform.h>
#include <Render/Render.h>

void GameObject::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
}