#include "Application.h"

#include "../Input/Events/InputEvents.h"

// external
#include <Events/Manager/EventsManager.h>	
#include <Timer/Timer.h>
#include <Logger/Logger.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// standard
#include <iostream>

// remove
#include <Entity/Entity.h>
#include <Components/Transform/Transform.h>
#include <Systems/Render/RenderSystem.h>

void Application::Initialize(const int& width, const int& height, const char* title, const bool& fullscreen) {
	// initialize GLFW
	if (!glfwInit()) {
		Console::Error << "GLFW INITIALISATION FAILED.\n";
		return;
	}

	context = new Window(width, height, title, fullscreen);
	inputController.Initialize(context->Get());

	// initialize GLEW
	glewExperimental = true;
	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		Console::Error << glewGetErrorString(err) << '\n';
	}

	Events::EventsManager::GetInstance()->Subscribe("EXIT", &Window::Close, context);
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("SCROLL_INPUT", &Application::OnEvent, this);

	entities = new EntityManager(10, 5);
	entities->Initialize();

	components = new ComponentsManager(10, 5);
	components->Add<Render>();

	systems = new SystemsManager;
	systems->Add<RenderSystem>();
}

void Application::Run() {
	Timer timer;
	timer.Start();

	while (!context->ShouldClose()) {
		glfwPollEvents();

		const float et = static_cast<float>(timer.GetElapsedTime());
		const float dt = static_cast<float>(timer.GetDeltaTime());
		const int FPS = static_cast<int>(1.f / dt);

		std::string title = "FPS : ";
		title += std::to_string(FPS);
		context->SetTitle(title.c_str());

		systems->Update(dt);

		context->SwapBuffers();
		timer.Update();
	}
}

void Application::Exit() {
	delete entities;
	delete components;
	delete systems;

	delete context;

	Events::EventsManager::Destroy();
}

void Application::OnEvent(Events::Event* event) {

	if (event->name == "KEY_INPUT") {
		Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
		// quit program if escaped
		if (input->key == GLFW_KEY_ESCAPE && input->action == GLFW_RELEASE) {
			Events::EventsManager::GetInstance()->Trigger("EXIT");
			return;
		}

		if (input->key == GLFW_KEY_0 && input->action == GLFW_RELEASE) {
			Entity* e = entities->Fetch();
			e->Use();
			e->AddComponent(components->Fetch<Render>());
			e->GetComponent<Render>()->SetActive(true);
			return;
		}
	} else if (event->name == "CURSOR_POSITION_INPUT") {
		Events::CursorPositionInput* input = static_cast<Events::CursorPositionInput*>(event);
	} else if (event->name == "MOUSE_BUTTON_INPUT") {
		Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
	} else if (event->name == "SCROLL_INPUT") {
		Events::ScrollInput* input = static_cast<Events::ScrollInput*>(event);
	}

}
