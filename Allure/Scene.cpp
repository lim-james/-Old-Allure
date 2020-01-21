#include "Scene.h"

#include "TransformSystem.h"

#include <Math/Vectors.hpp>
#include <Events/EventsManager.h>

Scene::Scene() {
	entities = new EntityManager;
	systems = new SystemManager(entities);
}

Scene::~Scene() {
	delete entities;
	delete systems;
}

void Scene::Awake() {
	systems->Subscribe<TransformSystem>(0);
}

void Scene::Reset() {
	Events::EventsManager::GetInstance()->SubscribeContext(this);
	systems->Start();
}

void Scene::Start() {}

void Scene::FixedUpdate(const float& dt) {}

void Scene::Update(const float& dt) {
	systems->Update(dt);
}

void Scene::Stop() {
	Events::EventsManager::GetInstance()->UnsubscribeContext(this);
	systems->Stop();
}

void Scene::Destroy() {}

void Scene::PrepareForSegue(Scene * destination) { }
