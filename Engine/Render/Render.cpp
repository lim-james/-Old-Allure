#include "Render.h"

#include <Events/EventsManager.h>

Render::Render() 
	: material(nullptr)
	, model(nullptr)
	, midModel(nullptr)
	, lowModel(nullptr) {}

Render::~Render() {

}

void Render::Initialize() {
	Component::Initialize();

	material = nullptr;
	model = nullptr;
	midModel = nullptr;
	lowModel = nullptr;
}

void Render::SetActive(const bool& state) {
	Component::SetActive(state);
	Events::EventsManager::GetInstance()->Trigger("RENDER_ACTIVE", new Events::AnyType<Render*>(this));
}
