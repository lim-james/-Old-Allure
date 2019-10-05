#include "Render.h"

#include "../Events/Manager/EventsManager.h"

Render::Render() 
	: model(nullptr) {}

Render::~Render() {

}

void Render::Initialize() {
	Component::Initialize();
}

void Render::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("RENDER_ACTIVE", new Events::AnyType<Render*>(this));
}
