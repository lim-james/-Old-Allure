#include "Render.h"

#include <Events/EventsManager.h>

Render::Render() 
	: material(nullptr)
	, model(nullptr) {}

Render::~Render() {

}

void Render::Initialize() {
	Component::Initialize();

	material = nullptr;
	model = nullptr;
}

void Render::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("RENDER_ACTIVE", new Events::AnyType<Render*>(this));
}
