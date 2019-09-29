#include "RenderSystem.h"

#include "../../Events/Manager/EventsManager.h"

#include <Logger/Logger.h>

RenderSystem::RenderSystem() {
	Events::EventsManager::GetInstance()->Subscribe("RENDER_ACTIVE", &RenderSystem::ActiveHandler, this);
}

RenderSystem::~RenderSystem() {
	components.clear();
}

void RenderSystem::Update(const float& t) {
	for (auto& c : components) {
		
	}
}

void RenderSystem::ActiveHandler(Events::Event* event) {
	const auto component = static_cast<Events::AnyType<Render*>*>(event);

	if (component->data->IsActive()) {
		components.insert(component->data);
		Console::Log << "Added a render component.\n";
	} else {
		components.erase(component->data);
	}
}