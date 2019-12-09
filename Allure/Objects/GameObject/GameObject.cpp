#include "GameObject.h"

#include <Components/Transform/Transform.h>
#include <Render/Render.h>
#include <Physics/Rigidbody.h>
#include <Physics/Collider/Collider.h>

void GameObject::Build() {
	AddComponent<Transform>();
	AddComponent<Render>();
	AddComponent<Rigidbody>();
	AddComponent<Collider>();
}