#include "SystemsManager.h"

SystemsManager::~SystemsManager() {
	for (const auto& s : systems)
		delete s.second;

	systems.clear();
}

void SystemsManager::Update(const float& t) {
	for (const auto& s : systems)
		s.second->Update(t);
}