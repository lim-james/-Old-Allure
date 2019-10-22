#include "Application.h"

#include "../Input/InputEvents.h"

#include "../Scenes/TestScene.h"
#include "../Scenes/StressScene.h"
#include "../Scenes/Scene2D.h"
#include "../Scenes/ExhibitionScene.h"

// external
#include <Events/EventsManager.h>	
#include <Logger/Logger.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// standard
#include <iostream>

#include <Render/Load/LoadOBJ.h>

Application::Application() 
	: context(nullptr) 
	, current(nullptr) {}

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

#if _DEBUG
	Events::EventsManager::GetInstance()->Subscribe("TIMER_START", &Application::OnTimerEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("TIMER_STOP", &Application::OnTimerEvent, this);
#endif

	// turn off vsync
	//glfwSwapInterval(0);

	current = new ExhibitionScene;

	context->BroadcastSize();
}

void Application::Run() {
	timer.Start();

	Events::EventsManager::GetInstance()->Trigger("CURSOR_SENSITIVITY", new Events::AnyType<float>(0.1f));
	Events::EventsManager::GetInstance()->Trigger("INPUT_MODE_CHANGE", new Events::InputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED));

	current->Awake();

	int highest = 0;

	while (!context->ShouldClose()) {
		glfwPollEvents();

		const float et = static_cast<float>(timer.GetElapsedTime());
		const float dt = static_cast<float>(timer.GetDeltaTime());
		const int FPS = static_cast<int>(1.f / dt);

		//if (FPS > highest) {
		//	highest = FPS;
		//	Console::Warn << "Highest FPS : " << highest << '\n';
		//}

		//if (FPS < 30) {
		//	Console::Error << "SHIT FPS : " << FPS << '\n';
		//} else if (FPS < 60) {
		//	Console::Error << "LOW FPS : " << FPS << '\n';
		//}

		std::string title = "FPS : ";
		title += std::to_string(FPS);
		context->SetTitle(title.c_str());

		current->Update(dt);

		context->SwapBuffers();
		timer.Update();
	}
}

void Application::Exit() {
#if _DEBUG
	timers.clear();
#endif

	delete context;
	delete current;

	Events::EventsManager::Destroy();

	Load::ClearModelCache();
}

void Application::OnEvent(Events::Event* event) {
	if (event->name == "KEY_INPUT") {
		Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
		// quit program if escaped
		if (input->key == GLFW_KEY_ESCAPE && input->action == GLFW_RELEASE) {
			Events::EventsManager::GetInstance()->Trigger("EXIT");
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

#if _DEBUG
void Application::OnTimerEvent(Events::Event* event) {
	const auto timer = static_cast<Events::AnyType<std::string>*>(event);

	if (event->name == "TIMER_START") {
		timers[timer->data].Start();
	} else if (event->name == "TIMER_STOP") {
		timers[timer->data].Update();
		Console::Log << "[TIMER] " << timer->data << " : " << timers[timer->data].GetElapsedTime() << '\n';
	}
}
#endif
