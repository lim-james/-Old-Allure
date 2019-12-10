#include "GameScene.h"

// Objects
#include "../Objects/Camera/CameraObject.h"
#include "../Objects/Camera/FlyingCamera.h"
#include "../Objects/Light/DirectionalLight.h"
#include "../Objects/Light/LightObject.h"
#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Player/PlayerObject.h"
// Components
#include <Components/Transform/Transform.h>
#include <Physics/Rigidbody.h>
#include <Physics/Collider/Collider.h>
// Systems
#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>
#include <Physics/PhysicsSystem.h>
// Utils
#include <Render/Load/LoadOBJ.h>

#include <GLFW/glfw3.h>

GameScene::GameScene() {

}

void GameScene::Awake() {
	Scene::Awake();

	components->Subscribe<Transform>(10, 5);
	components->Subscribe<Render>(10, 5);
	components->Subscribe<Light>(15, 1);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Script>(10, 5);
	components->Subscribe<Rigidbody>(5, 5);
	components->Subscribe<Collider>(5, 5);

	entities->Subscribe<CameraObject>(1, 1);
	entities->Subscribe<FlyingCamera>(1, 1);
	entities->Subscribe<DirectionalLight>(1, 0);
	entities->Subscribe<LightObject>(15, 0);
	entities->Subscribe<GameObject>(10, 5);
	entities->Subscribe<PlayerObject>(2, 1);

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();
	systems->Subscribe<PhysicsSystem>();

	normal = new Material::Standard;
	normal->tint.Set(1.0f, 1.0f, 1.0f);

	field = new Material::Standard;
	field->tint.Set(0.f, 1.f, 0.f);
}

void GameScene::Start() {
	Scene::Start();

	auto camera = entities->Create<FlyingCamera>();
	camera->GetComponent<Transform>()->translation.Set(0.0f, 5.0f, 0.0f);
	camera->GetComponent<Camera>()->clearColor.Set(0.0f);

	//auto fieldObject = entities->Create<GameObject>();
	//fieldObject->GetComponent<Transform>()->scale.Set(9.f, 1.f, 12.f);
	//fieldObject->GetComponent<Render>()->material = field;
	//fieldObject->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	//fieldObject->GetComponent<Rigidbody>()->hasGravity = false;
	//fieldObject->SetTag("wall");
	
	auto p1 = entities->Create<PlayerObject>();
	p1->SetForwardKey(GLFW_KEY_UP);
	p1->SetBackwardsKey(GLFW_KEY_DOWN);
	p1->GetComponent<Transform>()->translation.Set(0.5f, 1.f, 3.f);
	p1->GetComponent<Render>()->material = normal;
	p1->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	
	auto p2 = entities->Create<PlayerObject>();
	p2->GetComponent<Transform>()->translation.Set(-0.5f, 1.f, -3.f);
	p2->GetComponent<Render>()->material = normal;
	p2->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	
	auto ball = entities->Create<GameObject>();
	ball->GetComponent<Transform>()->translation.Set(0.f, 1.f, 0.3f);
	ball->GetComponent<Render>()->material = normal;
	ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	//ball->GetComponent<Rigidbody>()->hasGravity = false;
	ball->GetComponent<Rigidbody>()->mass = 10.f;
	ball->SetTag("ball");

	auto ball2 = entities->Create<GameObject>();
	ball2->GetComponent<Transform>()->translation.Set(20.f, 1.f, 0.f);
	ball2->GetComponent<Render>()->material = normal;
	ball2->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	ball2->GetComponent<Rigidbody>()->mass = 10.f;
	ball2->GetComponent<Rigidbody>()->velocity = vec3f(-5.f, 0, 0);
	ball2->SetTag("ball");

	//auto ball3 = entities->Create<GameObject>();
	//ball3->GetComponent<Transform>()->translation.Set(-200.f, 1.f, 0.f);
	//ball3->GetComponent<Render>()->material = normal;
	//ball3->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	//ball3->GetComponent<Rigidbody>()->mass = 10.f;
	//ball3->GetComponent<Rigidbody>()->velocity = vec3f(80.f, 0, 0);
	//ball3->SetTag("ball");

	auto ball4 = entities->Create<GameObject>();
	ball4->GetComponent<Transform>()->translation.Set(0.f, 20.f, 0.f);
	ball4->GetComponent<Render>()->material = normal;
	ball4->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	ball4->GetComponent<Rigidbody>()->mass = 10.f;
	ball4->GetComponent<Rigidbody>()->velocity = vec3f(0, -10.f, 0);
	ball4->SetTag("ball");

	{
		auto light = entities->Create<DirectionalLight>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 8.0f, -15.0f);
		light->GetComponent<Transform>()->rotation.Set(-30.0f, 0.0f, 0.0f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Light>()->specular.Set(0.f);
	}

	{
		auto light = entities->Create<LightObject>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 7.0f, 0.0f);
		light->GetComponent<Transform>()->scale.Set(0.1f);
		light->GetComponent<Transform>()->rotation.Set(-90.0f, 0.0f, 0.0f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Render>()->material = normal;
		light->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
		light->GetComponent<Light>()->type = Light::SPOT;
		light->GetComponent<Light>()->power = 5.f;
	}
}

void GameScene::Destroy() {
	Scene::Destroy();

	delete normal;
	delete field;
}
