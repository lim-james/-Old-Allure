#include "ObjectFactory.h"

ObjectFactory::ObjectFactory() {
	entities = new EntityManager;
	entities->Add<GameObject>(10, 5);
	entities->Add<CameraObject>(1, 1);
	entities->Add<LightObject>(16, 0);

	components = new ComponentsManager;
	components->Add<Transform>(10, 5);
	components->Add<Camera>(1, 1);
	components->Add<Render>(10, 5);
	components->Add<Light>(16, 0);
	components->Add<Script>(10, 5);

	systems = new SystemsManager;
	systems->Add<RenderSystem>();
	systems->Add<ScriptSystem>();
}

ObjectFactory::~ObjectFactory() {
	delete entities;
	delete components;
	delete systems;
}

void ObjectFactory::Update(const float& dt) {
	systems->Update(dt);
}