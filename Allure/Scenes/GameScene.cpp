#include "GameScene.h"

#include "../Input/InputEvents.h"

// Objects
#include "../Objects/Camera/CameraObject.h"
#include "../Objects/Camera/FlyingCamera.h"
#include "../Objects/Light/DirectionalLight.h"
#include "../Objects/Light/LightObject.h"
#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Player/PlayerObject.h"
// Components
#include <Components/Transform/Transform.h>
// Systems
#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>
#include <Physics/PhysicsSystem.h>
// Utils
#include <Render/Load/LoadOBJ.h>

#include <Events/EventsManager.h>

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

	entities->Subscribe<CameraObject>(1, 1);
	entities->Subscribe<FlyingCamera>(1, 1);
	entities->Subscribe<DirectionalLight>(1, 0);
	entities->Subscribe<LightObject>(15, 0);
	entities->Subscribe<GameObject>(10, 5);
	entities->Subscribe<PlayerObject>(2, 1);

	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();

	normal = new Material::Standard;
	normal->tint.Set(1.0f, 1.0f, 1.0f);

	field = new Material::Standard;
	field->tint.Set(0.f, 1.f, 0.f);
}

void GameScene::Reset() {
	Scene::Reset();
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &GameScene::MouseHandler, this);
}

void GameScene::Start() {
	Scene::Start();

	camera = entities->Create<FlyingCamera>();
	camera->GetComponent<Transform>()->translation.Set(0.0f, 5.0f, 0.0f);
	camera->GetComponent<Camera>()->clearColor.Set(0.0f);

	auto fieldObject = entities->Create<GameObject>();
	fieldObject->GetComponent<Transform>()->scale.Set(10.f, 1.f, 10.f);
	fieldObject->GetComponent<Render>()->material = field;
	fieldObject->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	ball = entities->Create<GameObject>();
	ball->GetComponent<Transform>()->translation.Set(0.f, 1.f, 0.f);
	ball->GetComponent<Render>()->material = normal;
	ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");

	{
		auto light = entities->Create<DirectionalLight>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 8.0f, -15.0f);
		light->GetComponent<Transform>()->rotation.Set(30.0f, 0.0f, 0.0f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Light>()->specular.Set(0.f);
	}

	{
		auto light = entities->Create<LightObject>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 7.0f, 0.0f);
		light->GetComponent<Transform>()->scale.Set(0.1f);
		light->GetComponent<Transform>()->rotation.Set(90.0f, 0.0f, 0.0f);
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

void GameScene::MouseHandler(Events::Event * event) {
	auto input = static_cast<Events::MouseButtonInput*>(event);
	if (input->action == GLFW_PRESS && input->button == GLFW_MOUSE_BUTTON_LEFT) {
		auto transform = ball->GetComponent<Transform>();
		if (ball->GetParent()) {
			transform->translation.Set(transform->GetWorldTranslation());
			ball->SetParent(nullptr);
		} else {
			transform->translation.Set(0.f, 0.f, 2.f);
			ball->SetParent(camera);
		}
	}
}
