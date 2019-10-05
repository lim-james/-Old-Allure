#include "Application.h"

#include "../Input/Events/InputEvents.h"

// external
#include <Events/Manager/EventsManager.h>	
#include <Logger/Logger.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// standard
#include <iostream>

// remove
#include "../Objects/Camera/CameraObject.h"
#include "../Objects/GameObject/GameObject.h"
#include <Render/Load/OBJ/LoadOBJ.h>

Application::Application() 
	: context(nullptr) 
	, factory(nullptr) {}

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

	factory = new ObjectFactory;

	{
		auto camera = factory->Create<CameraObject>();
		camera->GetComponent<Camera>()->clearColor.Set(vec3f(0.1f), 1.0f);
		camera->GetComponent<Camera>()->SetDepth(-0.5f);
		camera->GetComponent<Camera>()->viewportRect.Set(0.5f, 0.0f, 0.5f, 1.0f);
	}

	{
		auto camera = factory->Create<CameraObject>();
		camera->GetComponent<Camera>()->clearColor.Set(1.0f, 0.0f, 0.0f, 1.0f);
		camera->GetComponent<Camera>()->SetDepth(-1.0f);
		camera->GetComponent<Camera>()->viewportRect.Set(0.25f, 0.25f, 0.5f, 0.5f);
	}

	{
		auto camera = factory->Create<CameraObject>();
		camera->GetComponent<Camera>()->clearColor.Set(0.0f, 1.0f, 0.0f, 1.0f);
		camera->GetComponent<Camera>()->SetDepth(-0.4f);
		camera->GetComponent<Camera>()->viewportRect.Set(0.0f, 0.5f, 0.7f, 0.5f);
	}

	context->BroadcastSize();
}

void Application::Run() {
	timer.Start();

	Events::EventsManager::GetInstance()->Trigger("TIMER_START", new Events::AnyType<std::string>("INITIAL LOAD"));
	Load::OBJ("Files/Models/cube.obj");
	Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("INITIAL LOAD"));

	Events::EventsManager::GetInstance()->Trigger("TIMER_START", new Events::AnyType<std::string>("CACHED"));
	Load::OBJ("Files/Models/cube.obj");
	Events::EventsManager::GetInstance()->Trigger("TIMER_STOP", new Events::AnyType<std::string>("CACHED"));

	while (!context->ShouldClose()) {
		glfwPollEvents();

		const float et = static_cast<float>(timer.GetElapsedTime());
		const float dt = static_cast<float>(timer.GetDeltaTime());
		const int FPS = static_cast<int>(1.f / dt);

		std::string title = "FPS : ";
		title += std::to_string(FPS);
		context->SetTitle(title.c_str());

		factory->Update(dt);

		context->SwapBuffers();
		timer.Update();
	}
}

void Application::Exit() {
#if _DEBUG
	timers.clear();
#endif

	delete context;

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

		if (input->key == GLFW_KEY_0 && input->action == GLFW_RELEASE) {
			auto go = factory->Create<GameObject>();

			go->GetComponent<Transform>()->translation.Set(0.0f, 0.0f, 5.0f);
			go->GetComponent<Transform>()->rotation.Set(45.0f, 45.0f, 30.0f);
			go->GetComponent<Render>()->model = Load::OBJ("Files/Models/cube.obj");

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
