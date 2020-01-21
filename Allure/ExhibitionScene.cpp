#include "ExhibitionScene.h"

#include "../Objects/Camera/FlyingCamera.h"
#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Light/LightObject.h"

// Components
#include <Components/Transform/Transform.h>

// Systems
#include <Render/RenderSystem.h>
#include <Script/ScriptSystem.h>

// Util
#include <Render/Load/LoadOBJ.h>
#include <Render/Load/LoadTGA.h>
#include <Math/Random.hpp>

ExhibitionScene::ExhibitionScene() {
	// subscribe components for pooling
	components->Subscribe<Transform>(10, 5);
	components->Subscribe<Camera>(1, 1);
	components->Subscribe<Render>(10, 5);
	components->Subscribe<Light>(16, 0);
	components->Subscribe<Script>(10, 5);

	// subscribe entities for pooling
	entities->Subscribe<GameObject>(10, 5);
	entities->Subscribe<FlyingCamera>(1, 1);
	entities->Subscribe<LightObject>(15, 0);

	// subscribe systems
	systems->Subscribe<RenderSystem>();
	systems->Subscribe<ScriptSystem>();

	// initialising variables
	normal = nullptr;
}

void ExhibitionScene::Awake() {
	// Create spectator cam
	auto camera = entities->Create<FlyingCamera>();
	camera->GetComponent<Transform>()->translation.Set(0.f, 5.f, 0.f);

	normal = new Material::Standard;

	vec2f positions[4] = {
		vec2f( 10.0f,  10.0f),
		vec2f( 10.0f, -10.0f),
		vec2f(-10.0f,  10.0f),
		vec2f(-10.0f, -10.0f)
	};

	for (const auto& pos : positions) {
		auto light = entities->Create<LightObject>();
		light->GetComponent<Transform>()->translation.Set(pos.x, 10.f, pos.y);
		light->GetComponent<Transform>()->rotation.Set(-90.f, 0.f, 0.f);
		light->GetComponent<Transform>()->scale.Set(0.1f);
		light->GetComponent<Transform>()->UpdateLocalAxes();
		light->GetComponent<Render>()->material = normal;
		light->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
		light->GetComponent<Light>()->type = Light::POINT; // Change to SPOT for shadows
		light->GetComponent<Light>()->power = 10.f;

		auto platform = entities->Create<GameObject>();
		platform->GetComponent<Transform>()->translation.Set(pos.x, 0.f, pos.y);
		platform->GetComponent<Transform>()->scale.Set(15.f, 1.f, 15.f);
		platform->GetComponent<Render>()->material = normal;
		platform->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");
	}

	// Spheres for PLATFORM 1
	for (int i = 0; i < 100; ++i) 
	{
		const vec3f random(Math::RandMinMax(2.5f, 17.5f), 5.f, Math::RandMinMax(2.5f, 17.5f));
		auto sphere = entities->Create<GameObject>();
		sphere->GetComponent<Transform>()->translation.Set(random.x, random.y, random.z);
		sphere->GetComponent<Transform>()->scale.Set(0.5f);
		sphere->GetComponent<Render>()->material = normal;
		sphere->GetComponent<Render>()->model = Load::OBJ("Files/Models/sphere.obj");
		sphere->SetTag("balls");
	}

	// Spatial Partitioning Cube Visuals
	//for (int x = 0; x < 50; x++)
	//{
	//	for (int y = 0; y < 50; y++)
	//	{
	//		auto cube = entities->Create<GameObject>();
	//		cube->GetComponent<Transform>()->translation.Set(x * )
	//	}
	//}
}

void ExhibitionScene::Update(const float & dt)
{
	//for (const auto& ball : )
}

void ExhibitionScene::Destroy() {
	delete normal;
}