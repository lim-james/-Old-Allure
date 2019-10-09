#include "Scene.h"

#include "../Objects/Camera/CameraObject.h"
#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Light/LightObject.h"

// Components
#include <Components/Transform/Transform.h>

// Systems
#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>

// materials
#include <Render/Material/Standard/StandardMaterial.h>
#include <Render/Material/Nonlit/ColorMaterial.h>

#include <Render/Load/LoadOBJ.h>
#include <Render/Load/LoadTGA.h>

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

	container = new Material::Standard;
	container->albedo = Load::TGA("Files/Textures/container.tga");
	//container->tint.Set(1.0f, 0.0f, 0.0f);
	container->metallic = Load::TGA("Files/Textures/container_specular.tga");
	//container->smoothness = 1.0f;

	auto floor = entities->Create<GameObject>();
	floor->GetComponent<Transform>()->scale.Set(10.0f, 1.0f, 10.0f);
	floor->GetComponent<Render>()->material = container;
	floor->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto ball = entities->Create<GameObject>();
	ball->GetComponent<Transform>()->translation.Set(0.0f, 2.0f, 0.0f);
	ball->GetComponent<Render>()->material = container;
	ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");

	bulb = new Material::Color;

	auto light = entities->Create<LightObject>();
	light->GetComponent<Transform>()->translation.Set(0.0f, 5.0f, 0.0f);
	light->GetComponent<Transform>()->scale.Set(0.1f);
	light->GetComponent<Transform>()->rotation.Set(-89.f, 0.0f, 0.0f);
	light->GetComponent<Transform>()->UpdateLocalAxes();
	light->GetComponent<Render>()->material = bulb;
	light->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	light->GetComponent<Light>()->type = Light::SPOT;
	light->GetComponent<Light>()->power = 10.f;

	//auto dirlight = entities->Create<LightObject>();
	//dirlight->GetComponent<Transform>()->rotation.Set(-89.f, 0.0f, 0.0f);
	//dirlight->GetComponent<Transform>()->Update();
	//dirlight->GetComponent<Render>()->SetActive(false);
	//dirlight->GetComponent<Light>()->type = Light::DIRECTIONAL;
	//dirlight->GetComponent<Light>()->ambient.Set(0.01f);
	//dirlight->GetComponent<Light>()->diffuse.Set(0.1f);
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

	delete container;
	delete bulb;
}