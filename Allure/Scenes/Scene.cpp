#include "Scene.h"

#include "../Objects/Camera/CameraObject.h"
#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Light/LightObject.h"
#include "../Objects/Light/DirectionalLight.h"

// Components
#include <Components/Transform/Transform.h>

// Systems
#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>
#include <Physics/PhysicsSystem.h>

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
	components->Add<Rigidbody>(5, 5);

	entities = new EntityManager(components);
	entities->Add<GameObject>(10, 5);
	entities->Add<CameraObject>(1, 1);
	entities->Add<LightObject>(15, 0);
	entities->Add<DirectionalLight>(1, 0);

	systems = new SystemsManager;
	systems->Add<RenderSystem>();
	systems->Add<ScriptSystem>();
	systems->Add<PhysicsSystem>();
}

Scene::~Scene() {
	Destroy();
}

void Scene::Awake() {
	auto camera = entities->Create<CameraObject>();
	camera->GetComponent<Transform>()->translation.Set(0.0f, 5.0f, 0.0f);
	//camera->GetComponent<Camera>()->clearColor.Set(0.53f, 0.81f, 0.92f, 1.0f);
	camera->GetComponent<Camera>()->clearColor.Set(0.0f);

	normal = new Material::Standard;
	normal->tint.Set(1.0f, 1.0f, 1.0f);

	red = new Material::Standard;
	red->tint.Set(1.0f, 0.0f, 0.0f);

	green = new Material::Standard;
	green->tint.Set(0.0f, 1.0f, 0.0f);

	blue = new Material::Standard;
	blue->tint.Set(0.0f, 0.0f, 1.0f);

	auto floor = entities->Create<GameObject>();
	floor->GetComponent<Transform>()->scale.Set(10.f, 1.f, 10.f);
	floor->GetComponent<Render>()->material = normal;
	floor->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto ceiling = entities->Create<GameObject>();
	ceiling->GetComponent<Transform>()->translation.Set(0.f, 9.f, 0.f);
	ceiling->GetComponent<Transform>()->scale.Set(10.f, 1.f, 10.f);
	ceiling->GetComponent<Render>()->material = normal;
	ceiling->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto back = entities->Create<GameObject>();
	back->GetComponent<Transform>()->translation.Set(0.f, 4.5f, 4.5f);
	back->GetComponent<Transform>()->scale.Set(10.f, 8.f, 1.f);
	back->GetComponent<Render>()->material = normal;
	back->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto leftTop = entities->Create<GameObject>();
	leftTop->GetComponent<Transform>()->translation.Set(4.5f, 7.5f, 0.f);
	leftTop->GetComponent<Transform>()->scale.Set(1.f, 2.f, 5.f);
	leftTop->GetComponent<Render>()->material = blue;
	leftTop->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto leftBottom = entities->Create<GameObject>();
	leftBottom->GetComponent<Transform>()->translation.Set(4.5f, 2.f, 0.f);
	leftBottom->GetComponent<Transform>()->scale.Set(1.f, 5.f, 5.f);
	leftBottom->GetComponent<Render>()->material = blue;
	leftBottom->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto right = entities->Create<GameObject>();
	right->GetComponent<Transform>()->translation.Set(-4.5f, 4.5f, 0.f);
	right->GetComponent<Transform>()->scale.Set(1.f, 8.f, 10.f);
	right->GetComponent<Render>()->material = red;
	right->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	auto ball = entities->Create<GameObject>();
	ball->GetComponent<Transform>()->translation.Set(2.0f, 3.0f, 0.0f);
	ball->GetComponent<Transform>()->scale.Set(2.0f);
	ball->GetComponent<Render>()->material = red;
	ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	//ball->GetComponent<Rigidbody>()->

	auto box = entities->Create<GameObject>();
	box->GetComponent<Transform>()->translation.Set(-2.f, 1.5f, -2.0f);
	box->GetComponent<Transform>()->scale.Set(2.0f);
	box->GetComponent<Render>()->material = green;
	box->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	{
		auto light = entities->Create<DirectionalLight>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 8.0f, -15.0f);
		light->GetComponent<Transform>()->rotation.Set(-30.0f, 0.0f, 0.0f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Light>()->ambient.Set(0.f);
	}

	{
		auto light = entities->Create<LightObject>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 7.0f, 0.0f);
		light->GetComponent<Transform>()->scale.Set(0.1f);
		light->GetComponent<Transform>()->rotation.Set(-90.0f, 0.0f, 0.0f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Render>()->material = normal;
		light->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
		//light->GetComponent<Render>()->SetActive(false);
		light->GetComponent<Light>()->type = Light::SPOT;
		light->GetComponent<Light>()->ambient.Set(0.7f);
		light->GetComponent<Light>()->specular.Set(0.0f);
		light->GetComponent<Light>()->power = 5.f;
	}
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

	delete normal;
	delete red;
	delete green;
	delete blue;
}