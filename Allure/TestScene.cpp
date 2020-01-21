#include "TestScene.h"

#include "InputEvents.h"

// Components
#include <Transform.h>
// Systems
#include <RenderSystem.h>
// Util
#include <LoadOBJ.h>
#include <LoadTGA.h>
// Events
#include <Events/EventsManager.h>

#include <GLFW/glfw3.h>

void TestScene::Awake() {
	Scene::Awake();

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &TestScene::KeyInputHandler, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &TestScene::CursorPositionHandler, this);

	et = 0.f;

	normal = new Material::Standard;
	normal->tint.Set(1.0f, 1.0f, 1.0f);

	red = new Material::Standard;
	red->tint.Set(1.0f, 0.0f, 0.0f);

	green = new Material::Standard;
	green->tint.Set(0.0f, 1.0f, 0.0f);

	blue = new Material::Standard;
	blue->tint.Set(0.0f, 0.0f, 1.0f);

	movement.Set(0.f);
	cameraSpeed = 10.f;

	systems->Subscribe<RenderSystem>(0);

	auto cameraObject = entities->Create();
	cameraTransform = entities->GetComponent<Transform>(cameraObject);
	cameraTransform->translation.Set(0.0f, 5.0f, 0.0f);
	auto camera = entities->AddComponent<Camera>(cameraObject);
	camera->SetActive(true);
	camera->clearColor.Set(0.0f);

	CreateCube(vec3f(0.f), vec3f(10.f, 1.f, 10.f), normal);
	CreateCube(vec3f(0.f, 9.f, 0.f), vec3f(10.f, 1.f, 10.f), normal);
	CreateCube(vec3f(0.f, 4.5f, 4.5f), vec3f(10.f, 8.f, 1.f), normal);
	CreateCube(vec3f(4.5f, 7.5f, 0.f), vec3f(1.f, 2.f, 5.f), blue);
	CreateCube(vec3f(4.5f, 2.f, 0.f), vec3f(1.f, 5.f, 5.f), blue);
	CreateCube(vec3f(-4.5f, 4.5f, 0.f), vec3f(1.f, 8.f, 10.f), red);
	CreateCube(vec3f(-2.f, 1.5f, -2.0f), vec3f(2.f), green);
	CreateSphere(vec3f(2.0f, 3.0f, 0.0f), vec3f(2.f), red);

	auto dLight = CreateDirectionalLight(vec3f(0.0f, 8.0f, -15.0f), vec3f(30.0f, 0.0f, 0.0f));
	directionalLight = entities->GetComponent<Transform>(dLight->entity);

	auto light = CreateSpotLight(vec3f(0.0f, 7.0f, 0.0f), vec3f(90.0f, 0.0f, 0.0f));
	light->power = 5.f;
}

void TestScene::Start() {
	Scene::Start();

	Events::EventsManager::GetInstance()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED));
}

void TestScene::Update(const float & dt) {
	Scene::Update(dt);

	et += dt;

	const float angle = et * 30.f;
	directionalLight->translation.x = sin(Math::Rad(angle));
	directionalLight->translation.z = cos(Math::Rad(angle));
	directionalLight->rotation.y = 180.f + angle;
	directionalLight->UpdateAxes();

	const auto velocity = movement * cameraSpeed * dt;
	cameraTransform->translation += cameraTransform->GetLocalFront() * velocity.y;
	cameraTransform->translation += cameraTransform->GetLocalRight() * velocity.x;
}

void TestScene::Destroy() {
	Scene::Destroy();

	delete normal;
	delete red;
	delete green;
	delete blue;
}

void TestScene::KeyInputHandler(Events::Event * event) {
	auto input = static_cast<Events::KeyInput*>(event);

	vec2f direction(0.f);

	switch (input->key) {
	case GLFW_KEY_W:
		direction.y = 1.f;
		break;
	case GLFW_KEY_A:
		direction.x = -1.f;
		break;
	case GLFW_KEY_S:
		direction.y = -1.f;
		break;
	case GLFW_KEY_D:
		direction.x = 1.f;
		break;
	default:
		return;
	}

	switch (input->action) {
	case GLFW_PRESS:
		movement += direction;
		break;
	case GLFW_RELEASE:
		movement -= direction;
		break;
	default:
		break;
	}
}

void TestScene::CursorPositionHandler(Events::Event * event) {
	auto input = static_cast<Events::CursorPositionInput*>(event);
	cameraTransform->rotation.y -= input->offset.x;
	cameraTransform->rotation.x -= input->offset.y;
	cameraTransform->rotation.x = Math::Clamp(cameraTransform->rotation.x, -89.f, 89.f);
	cameraTransform->UpdateAxes();
}

void TestScene::CreateCube(const vec3f & translation, const vec3f & scale, Material::Base * material) {
	auto object = entities->Create();

	auto transform = entities->GetComponent<Transform>(object);
	transform->translation = translation;
	transform->scale = scale;

	auto render = entities->AddComponent<Render>(object);
	render->SetActive(true);
	render->material = material;
	render->model = Load::OBJ("Files/Models/cube.obj");
}

void TestScene::CreateSphere(const vec3f & translation, const vec3f & scale, Material::Base * material) {
	auto object = entities->Create();

	auto transform = entities->GetComponent<Transform>(object);
	transform->translation = translation;
	transform->scale = scale;

	auto render = entities->AddComponent<Render>(object);
	render->SetActive(true);
	render->material = material;
	render->model = Load::OBJ("Files/Models/sphere.obj");
}

Light * TestScene::CreateDirectionalLight(const vec3f & translation, const vec3f & rotation) {
	auto object = entities->Create();

	auto transform = entities->GetComponent<Transform>(object);
	transform->translation = translation;
	transform->rotation = rotation;
	transform->UpdateAxes();

	auto light = entities->AddComponent<Light>(object);
	light->SetActive(true);
	light->type = Light::DIRECTIONAL;

	return light;
}

Light * TestScene::CreateSpotLight(const vec3f & translation, const vec3f & rotation) {
	auto object = entities->Create();

	auto transform = entities->GetComponent<Transform>(object);
	transform->translation = translation;
	transform->rotation = rotation;
	transform->scale.Set(0.1f);
	transform->UpdateAxes();

	auto render = entities->AddComponent<Render>(object);
	render->SetActive(true);
	render->material = normal;
	render->model = Load::OBJ("Files/Models/cube.obj");

	auto light = entities->AddComponent<Light>(object);
	light->SetActive(true);
	light->type = Light::SPOT;

	return light;
}

