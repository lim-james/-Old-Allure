#include "StressScene.h"

#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Camera/FlyingCamera.h"

#include <Components/Transform/Transform.h>
#include <Components/Camera/Camera.h>

#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>

#include <Render/Load/LoadOBJ.h>

StressScene::StressScene() {
	components->Subscribe<Transform>(10, 5);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Render>(10, 5);
	components->Subscribe<Script>(10, 5);

	entities->Subscribe<GameObject>(10, 5);
	entities->Subscribe<FlyingCamera>(1, 1);

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();

	normal = nullptr;
}

void StressScene::Awake() {
	normal = new Material::Color;

	auto camera = entities->Create<CameraObject>();
	
	float size = 5;
	
	for (float x = -size; x <= size; ++x) {
		for (float y = -size; y <= size; ++y) {
			for (float z = -size; z <= size; ++z) {
				auto box = entities->Create<GameObject>();
				box->GetComponent<Transform>()->translation.Set(x * 2.f, y * 2.f, z * 2.f);
				box->GetComponent<Render>()->material = normal;
				box->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
			}
		}
	}

}