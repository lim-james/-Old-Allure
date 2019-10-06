#include "Scene.h"

#include "../Objects/Camera/CameraObject.h"
#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Light/LightObject.h"

// Components
#include <Components/Transform/Transform.h>

// Systems
#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>

#include <Render/Load/LoadOBJ.h>

Scene::Scene() {
	components = new ComponentsManager;
	components->Add<Transform>(10, 5);
	components->Add<Camera>(1, 1);
	components->Add<Render>(10, 5);
	components->Add<Light>(16, 0);
	components->Add<Script>(10, 5);

	entities = new EntityManager(components);
	entities->Add<GameObject>(10, 5);
	entities->Add<CameraObject>(1, 1);
	entities->Add<LightObject>(16, 0);

	systems = new SystemsManager;
	systems->Add<RenderSystem>();
	systems->Add<ScriptSystem>();
}

Scene::~Scene() {
	Destroy();
}

void Scene::Awake() {
	auto camera = entities->Create<CameraObject>();
	camera->GetComponent<Transform>()->translation.Set(0.0f, 5.0f, 0.0f);
	camera->GetComponent<Camera>()->clearColor.Set(0.0f);

	auto floor = entities->Create<GameObject>();
	floor->GetComponent<Transform>()->scale.Set(10.0f, 1.0f, 10.0f);
	floor->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto block = entities->Create<GameObject>();
	block->GetComponent<Transform>()->translation.Set(0.0f, 2.0f, 0.0f);
	block->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto light = entities->Create<LightObject>();
	light->GetComponent<Transform>()->translation.Set(0.0f, 4.0f, 0.0f);
	light->GetComponent<Transform>()->scale.Set(0.1f);
	light->GetComponent<Transform>()->rotation.Set(-89.f, 0.0f, 0.0f);
	light->GetComponent<Transform>()->Update();
	light->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	light->GetComponent<Light>()->type = Light::SPOT;
}

void Scene::Reset() {
	 
}

void Scene::Start() {

}

void Scene::FixedUpdate() {

}

void Scene::Update(const float& dt) {
	systems->Update(dt);
}

void Scene::Destroy() {
	delete entities;
	delete components;
	delete systems;
}