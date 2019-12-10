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
#include <Physics/Rigidbody.h>
#include <Physics/Collider/Collider.h>
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

void GameScene::Reset() {
	Scene::Reset();
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &GameScene::MouseHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &GameScene::KeyHandler, this);
}

void GameScene::Start() {
	Scene::Start();
	
	auto fixedCamera = entities->Create<CameraObject>();
	fixedCamera->GetComponent<Transform>()->translation.Set(0.0f, 10.0f, 0.0f);
	fixedCamera->GetComponent<Transform>()->rotation.Set(90.0f, 0.f, 0.0f);
	fixedCamera->GetComponent<Transform>()->UpdateLocalAxes();
	fixedCamera->GetComponent<Camera>()->clearColor.Set(0.0f);
	fixedCamera->GetComponent<Camera>()->SetViewportRect(vec4f(vec2f(0.8f), vec2f(0.2f)));
	fixedCamera->GetComponent<Camera>()->SetDepth(1.f);

	camera = entities->Create<FlyingCamera>();
	camera->GetComponent<Transform>()->translation.Set(0.0f, 5.0f, 0.0f);
	camera->GetComponent<Camera>()->clearColor.Set(0.0f);

	auto fieldObject = entities->Create<GameObject>();
	fieldObject->GetComponent<Transform>()->scale.Set(10.f, 1.f, 10.f);
	fieldObject->GetComponent<Render>()->material = field;
	fieldObject->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

	ball = entities->Create<GameObject>();
	ball->SetTag("ball");
	ball->GetComponent<Transform>()->translation.Set(0.f, 1.f, 0.f);
	ball->GetComponent<Render>()->material = normal;
	ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");

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
	if (input->action == GLFW_PRESS) {
		if (input->button == GLFW_MOUSE_BUTTON_LEFT) {
			auto transform = ball->GetComponent<Transform>();
			if (ball->GetParent()) {
				transform->translation.Set(transform->GetWorldTranslation());
				ball->SetParent(nullptr);
			} else {
				transform->translation.Set(0.f, 0.f, 2.f);
				ball->GetComponent<Rigidbody>()->velocity.Set(0.f);
				ball->SetParent(camera);
			}
		} else if (input->button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (ball->GetParent()) {
				ball->GetComponent<Transform>()->translation.Set(ball->GetComponent<Transform>()->GetWorldTranslation());
				ball->SetParent(nullptr);
				ball->GetComponent<Rigidbody>()->velocity = camera->GetComponent<Transform>()->GetLocalFront() * 5.f;
			}
		}
	}
}

void GameScene::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);
	if (input->action == GLFW_PRESS && input->key == GLFW_KEY_ENTER) {
		auto transform = ball->GetComponent<Transform>();
		if (ball->GetParent()) {
			transform->translation.Set(transform->GetWorldTranslation());
			ball->SetParent(nullptr);
		}

		ball = entities->Create<GameObject>();
		ball->SetTag("ball");
		ball->SetParent(camera);
		ball->GetComponent<Transform>()->translation.Set(0.f, 0.f, 2.f);
		ball->GetComponent<Render>()->material = normal;
		ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	}
}
