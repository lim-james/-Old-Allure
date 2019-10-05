#include "ObjectFactory.h"

#include <Render/System/RenderSystem.h>

ObjectFactory::ObjectFactory() {
	entities = new EntityManager;
	entities->Add<GameObject>(10, 5);
	entities->Add<CameraObject>(1, 1);
	entities->Initialize();

	components = new ComponentsManager;
	components->Add<Transform>(10, 5);
	components->Add<Camera>(1, 1);
	components->Add<Render>(10, 5);
	components->Initialize();

	systems = new SystemsManager;
	systems->Add<RenderSystem>();
}


ObjectFactory::~ObjectFactory() {
	delete entities;
	delete components;
	delete systems;
}

void ObjectFactory::Update(const float& dt) {
	systems->Update(dt);
}