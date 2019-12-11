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
#include <Math/Random.hpp>

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
	Events::EventsManager::GetInstance()->Subscribe("INDICES_COUNT", &GameScene::IndicesHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("FRUSTRUM_CULL_COUNT", &GameScene::CullHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("COLLISION_COUNT", &GameScene::CollisionHandler, this);
}


void GameScene::Start() {
	Scene::Start();

	auto fixedCamera = entities->Create<CameraObject>();
	fixedCamera->GetComponent<Transform>()->translation.Set(0.0f, 15.0f, 0.0f);
	fixedCamera->GetComponent<Transform>()->rotation.Set(90.0f, 0.f, 0.0f);
	fixedCamera->GetComponent<Transform>()->UpdateLocalAxes();
	fixedCamera->GetComponent<Camera>()->clearColor.Set(0.0f);
	fixedCamera->GetComponent<Camera>()->SetViewportRect(vec4f(vec2f(0.8f), vec2f(0.2f)));
	fixedCamera->GetComponent<Camera>()->SetDepth(1.f);

	camera = entities->Create<FlyingCamera>();
	camera->GetComponent<Transform>()->translation.Set(0.0f, 5.0f, 0.0f);
	camera->GetComponent<Camera>()->clearColor.Set(0.0f);

	ball = entities->Create<GameObject>();
	ball->SetTag("ball");
	ball->GetComponent<Transform>()->translation.Set(0.f, 1.f, 0.f);
	ball->GetComponent<Render>()->material = normal;
	ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	ball->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/sphere_mid.obj");
	ball->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/sphere_low.obj");

	//for (float z = -3.f; z <= 3.f; ++z) {
	//	for (float x = -3.f; x <= 3.f; ++x) {
	//		auto ball = entities->Create<GameObject>();
	//		ball->SetTag("ball");
	//		ball->GetComponent<Transform>()->translation.Set(x * 2.f, 1.f, z * 2.f);
	//		ball->GetComponent<Render>()->material = normal;
	//		ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	//	}
	//}

	//float size = 7.5f;

	//for (unsigned i = 0; i < 50; ++i) {
	//	const float radius = Math::RandMinMax(0.5f, 2.f);
	//	auto ball = entities->Create<GameObject>();
	//	ball->SetTag("ball");
	//	ball->GetComponent<Transform>()->translation.Set(Math::RandMinMax(-size, size), radius * 0.5f, Math::RandMinMax(-size, size));
	//	ball->GetComponent<Transform>()->scale.Set(radius);
	//	ball->GetComponent<Render>()->material = normal;
	//	ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	//	ball->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/sphere_mid.obj");
	//	ball->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/sphere_low.obj");
	//}

	//auto ball3 = entities->Create<GameObject>();
	//ball3->GetComponent<Transform>()->translation.Set(-200.f, 1.f, 0.f);
	//ball3->GetComponent<Render>()->material = normal;
	//ball3->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	//ball3->GetComponent<Rigidbody>()->mass = 10.f;
	//ball3->GetComponent<Rigidbody>()->velocity = vec3f(80.f, 0, 0);
	//ball3->SetTag("ball");

	//auto ball4 = entities->Create<GameObject>();
	//ball4->GetComponent<Transform>()->translation.Set(0.f, 20.f, 0.f);
	//ball4->GetComponent<Render>()->material = normal;
	//ball4->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
	//ball4->GetComponent<Rigidbody>()->mass = 10.f;
	//ball4->GetComponent<Rigidbody>()->velocity = vec3f(0, -10.f, 0);
	//ball4->SetTag("ball");


	floor = entities->Create<GameObject>();
	floor->GetComponent<Transform>()->translation.Set(0.f, -1.f, 0.f);
	floor->GetComponent<Transform>()->scale.Set(20.f, 1.f, 20.f);
	floor->GetComponent<Render>()->material = field;
	floor->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	floor->GetComponent<Render>()->SetActive(false);
	floor->GetComponent<Collider>()->normal = vec3f(0, 1, 0);
	floor->GetComponent<Collider>()->bounds->SetBounds(floor->GetComponent<Transform>()->translation, floor->GetComponent<Transform>()->scale, vec3i(0, 1, 0));
	floor->GetComponent<Rigidbody>()->hasGravity = false;
	floor->SetTag("wall");

	auto wallObject1 = entities->Create<GameObject>();
	wallObject1->GetComponent<Transform>()->translation.Set(0.f, 5.f, -10.f);
	wallObject1->GetComponent<Transform>()->scale.Set(20.f, 10.f, 1.f);
	wallObject1->GetComponent<Render>()->material = field;
	wallObject1->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	wallObject1->GetComponent<Render>()->SetActive(false);
	wallObject1->GetComponent<Collider>()->normal = vec3f(0, 0, 1);
	wallObject1->GetComponent<Collider>()->bounds->SetBounds(wallObject1->GetComponent<Transform>()->translation, wallObject1->GetComponent<Transform>()->scale, vec3i(0, 0, 1));
	wallObject1->GetComponent<Rigidbody>()->hasGravity = false;
	wallObject1->SetTag("wall");

	auto wallObject2 = entities->Create<GameObject>();
	wallObject2->GetComponent<Transform>()->translation.Set(0.f, 5.f, 10.f);
	wallObject2->GetComponent<Transform>()->scale.Set(20.f, 10.f, 1.f);
	wallObject2->GetComponent<Render>()->material = field;
	wallObject2->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	wallObject2->GetComponent<Render>()->SetActive(false);
	wallObject2->GetComponent<Collider>()->normal = vec3f(0, 0, -1);
	wallObject2->GetComponent<Collider>()->bounds->SetBounds(wallObject2->GetComponent<Transform>()->translation, wallObject2->GetComponent<Transform>()->scale, vec3i(0, 0, -1));
	wallObject2->GetComponent<Rigidbody>()->hasGravity = false;
	wallObject2->SetTag("wall");

	auto wallObject3 = entities->Create<GameObject>();
	wallObject3->GetComponent<Transform>()->translation.Set(-10.f, 5.f, 0.f);
	wallObject3->GetComponent<Transform>()->scale.Set(1.f, 10.f, 20.f);
	wallObject3->GetComponent<Render>()->material = field;
	wallObject3->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	wallObject3->GetComponent<Render>()->SetActive(false);
	wallObject3->GetComponent<Collider>()->normal = vec3f(1, 0, 0);
	wallObject3->GetComponent<Collider>()->bounds->SetBounds(wallObject3->GetComponent<Transform>()->translation, wallObject3->GetComponent<Transform>()->scale, vec3i(1, 0, 0));
	wallObject3->GetComponent<Rigidbody>()->hasGravity = false;
	wallObject3->SetTag("wall");

	auto wallObject4 = entities->Create<GameObject>();
	wallObject4->GetComponent<Transform>()->translation.Set(10.f, 5.f, 0.f);
	wallObject4->GetComponent<Transform>()->scale.Set(1.f, 10.f, 20.f);
	wallObject4->GetComponent<Render>()->material = field;
	wallObject4->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	wallObject4->GetComponent<Render>()->SetActive(false);
	wallObject4->GetComponent<Collider>()->normal = vec3f(-1, 0, 0);
	wallObject4->GetComponent<Collider>()->bounds->SetBounds(wallObject4->GetComponent<Transform>()->translation, wallObject4->GetComponent<Transform>()->scale, vec3i(-1, 0, 0));
	wallObject4->GetComponent<Rigidbody>()->hasGravity = false;
	wallObject4->SetTag("wall");

	//auto fieldObject1 = entities->Create<GameObject>();
	//fieldObject1->GetComponent<Transform>()->translation.Set(0.f, -15.f, 0.f);
	//fieldObject1->GetComponent<Transform>()->scale.Set(20.f, 1.f, 20.f);
	//fieldObject1->GetComponent<Render>()->material = field;
	//fieldObject1->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	//fieldObject1->GetComponent<Render>()->SetActive(false);
	//fieldObject1->GetComponent<Collider>()->normal = vec3f(0, 1, 0);
	//fieldObject1->GetComponent<Collider>()->bounds->SetBounds(fieldObject1->GetComponent<Transform>()->translation, fieldObject1->GetComponent<Transform>()->scale, vec3i(0, 1, 0));
	//fieldObject1->GetComponent<Rigidbody>()->hasGravity = false;
	//fieldObject1->SetTag("wall");

	auto cylinder1 = entities->Create<GameObject>();
	cylinder1->GetComponent<Transform>()->translation.Set(10.f, 5.f, 10.f);
	cylinder1->GetComponent<Transform>()->scale.Set(1.f, 1.f, 1.f);
	cylinder1->GetComponent<Render>()->material = normal;
	cylinder1->GetComponent<Render>()->model = Load::OBJ("Files/Models/cylinder.obj");
	cylinder1->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/cylinder_mid.obj");
	cylinder1->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/cylinder_low.obj");
	cylinder1->GetComponent<Rigidbody>()->hasGravity = false;
	cylinder1->SetTag("none");

	auto cylinder2 = entities->Create<GameObject>();
	cylinder2->GetComponent<Transform>()->translation.Set(-10.f, 5.f, 10.f);
	cylinder2->GetComponent<Transform>()->scale.Set(1.f, 1.f, 1.f);
	cylinder2->GetComponent<Render>()->material = normal;
	cylinder2->GetComponent<Render>()->model = Load::OBJ("Files/Models/cylinder.obj");
	cylinder2->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/cylinder_mid.obj");
	cylinder2->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/cylinder_low.obj");
	cylinder2->GetComponent<Rigidbody>()->hasGravity = false;
	cylinder2->SetTag("none");

	auto cylinder3 = entities->Create<GameObject>();
	cylinder3->GetComponent<Transform>()->translation.Set(-10.f, 5.f, -10.f);
	cylinder3->GetComponent<Transform>()->scale.Set(1.f, 1.f, 1.f);
	cylinder3->GetComponent<Render>()->material = normal;
	cylinder3->GetComponent<Render>()->model = Load::OBJ("Files/Models/cylinder.obj");
	cylinder3->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/cylinder_mid.obj");
	cylinder3->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/cylinder_low.obj");
	cylinder3->GetComponent<Rigidbody>()->hasGravity = false;
	cylinder3->SetTag("none");

	auto cylinder4 = entities->Create<GameObject>();
	cylinder4->GetComponent<Transform>()->translation.Set(10.f, 5.f, -10.f);
	cylinder4->GetComponent<Transform>()->scale.Set(1.f, 1.f, 1.f);
	cylinder4->GetComponent<Render>()->material = normal;
	cylinder4->GetComponent<Render>()->model = Load::OBJ("Files/Models/cylinder.obj");
	cylinder4->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/cylinder_mid.obj");
	cylinder4->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/cylinder_low.obj");
	cylinder4->GetComponent<Rigidbody>()->hasGravity = false;
	cylinder4->SetTag("none");

	{
		auto light = entities->Create<DirectionalLight>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 8.0f, -15.0f);
		light->GetComponent<Transform>()->rotation.Set(30.0f, 0.0f, 0.0f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Light>()->specular.Set(0.f);
	}

	{
		auto light = entities->Create<LightObject>();
		light->GetComponent<Transform>()->translation.Set(0.0f, 15.0f, 0.0f);
		light->GetComponent<Transform>()->scale.Set(0.1f);
		light->GetComponent<Transform>()->rotation.Set(90.0f, 0.0f, 0.0f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Render>()->material = normal;
		light->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
		light->GetComponent<Light>()->type = Light::SPOT;
		light->GetComponent<Light>()->power = 20.f;
	}
}

void GameScene::Update(const float & dt) {
	Scene::Update(dt);

	nocapFps = static_cast<int>(1.f / dt);
}

void GameScene::FixedUpdate(const float & dt) {
	Scene::FixedUpdate(dt);

	debugText = "FPS: " + std::to_string(nocapFps) + " (" + std::to_string(int(1.f / dt))  + ")";
	debugText += "\nINDICIES DRAWN: " + std::to_string(indicesCount);
	debugText += "\nFRUSTRUM CULL: " + std::string(fCull ? "TRUE" : "FALSE");
	debugText += "\nFRUSTRUM CHECKS: " + std::to_string(frustrumChecks);
	debugText += "\nLEVEL OF DETAIL: " + std::string(LOD ? "TRUE" : "FALSE");
	debugText += "\nSPATIAL PARTITION: " + std::string(partition ? "TRUE" : "FALSE");
	debugText += "\nCOLLISION CHECKS: " + std::to_string(collisionChecks);
	Events::EventsManager::GetInstance()->Trigger("DEBUG_TEXT", new Events::AnyType<std::string>(debugText));
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
				ball->GetComponent<Rigidbody>()->hasGravity = true;
			} else {
				transform->translation.Set(0.f, 0.f, 2.f);
				ball->GetComponent<Rigidbody>()->velocity.Set(0.f);
				ball->SetParent(camera);
				ball->GetComponent<Rigidbody>()->hasGravity = false;
			}
		} else if (input->button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (ball->GetParent()) {
				ball->GetComponent<Transform>()->translation.Set(ball->GetComponent<Transform>()->GetWorldTranslation());
				ball->SetParent(nullptr);
				ball->GetComponent<Rigidbody>()->velocity = camera->GetComponent<Transform>()->GetLocalFront() * 70.f;
				ball->GetComponent<Rigidbody>()->hasGravity = true;
			}
		}
	}
}

void GameScene::KeyHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);
	if (input->action == GLFW_PRESS) {
		if (input->key == GLFW_KEY_ENTER) {
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
			ball->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/sphere_mid.obj");
			ball->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/sphere_low.obj");
		} else if (input->key == GLFW_KEY_0) {
			fast = !fast;
			glfwSwapInterval(fast);
		} else if (input->key == GLFW_KEY_1) {
			fCull = !fCull;
			Events::EventsManager::GetInstance()->Trigger("FRUSTRUM_CULL", new Events::AnyType<bool>(fCull));
		} else if (input->key == GLFW_KEY_2) {
			partition = !partition;
			Events::EventsManager::GetInstance()->Trigger("PARTITION", new Events::AnyType<bool>(partition));
		} else if (input->key == GLFW_KEY_3) {
			LOD = !LOD;
			Events::EventsManager::GetInstance()->Trigger("LOD", new Events::AnyType<bool>(LOD));
		} else if (input->key == GLFW_KEY_6) {
			ball = entities->Create<GameObject>();
			ball->SetTag("ball");
			ball->GetComponent<Transform>()->translation.Set(Math::RandMinMax(-3, 3), 10.f, Math::RandMinMax(-3, 3));
			float rand = Math::RandMinMax(-3.f, 3.f);
			ball->GetComponent<Transform>()->scale.Set(rand, rand, rand);
			ball->GetComponent<Render>()->material = normal;
			ball->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
			ball->GetComponent<Render>()->midModel = Load::OBJ("Files/Models/sphere_mid.obj");
			ball->GetComponent<Render>()->lowModel = Load::OBJ("Files/Models/sphere_low.obj");
		} else if (input->key == GLFW_KEY_7) {
			//floor->GetComponent<Render>()->SetActive(false);
			floor->SetTag("others");
		}
	}
}

void GameScene::IndicesHandler(Events::Event * event) {
	indicesCount = static_cast<Events::AnyType<int>*>(event)->data;
}

void GameScene::CullHandler(Events::Event * event) {
	frustrumChecks = static_cast<Events::AnyType<int>*>(event)->data;
}

void GameScene::CollisionHandler(Events::Event * event) {
	collisionChecks = static_cast<Events::AnyType<int>*>(event)->data;
}
