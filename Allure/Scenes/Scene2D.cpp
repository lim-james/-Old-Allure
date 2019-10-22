#include "Scene2D.h"

#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Camera/CameraObject.h"

#include <Components/Transform/Transform.h>
#include <Components/Camera/Camera.h>

#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>

#include <Render/Material/Nonlit/ColorMaterial.h>

#include <Render/Load/LoadOBJ.h>

Scene2D::Scene2D() {
	components->Add<Transform>(10, 5);
	components->Add<Camera>(1, 1);
	components->Add<Render>(10, 5);
	components->Add<Script>(1, 1);

	entities->Add<GameObject>(10, 5);
	entities->Add<CameraObject>(10, 5);

	systems->Add<RenderSystem>();
	systems->Add<ScriptSystem>();
}

void Scene2D::Awake() {
	auto camera = entities->Create<CameraObject>();
	camera->GetComponent<Camera>()->projection = ORTHOGRAPHIC;
	
	auto box = entities->Create<GameObject>();
	box->GetComponent<Transform>()->translation.Set(0.f, 0.f, 1.f);
	box->GetComponent<Render>()->material = new Material::Color;
	box->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
}
