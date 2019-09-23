#include "Application.h"

#undef APIENTRY

#include "../Timer/Timer.h"
#include "../Events/Manager/EventsManager.h"	

#include "../Input/Events/InputEvents.h"

#include "../Logger/Logger.h"

Application::Application() 
	: context(nullptr)
{}

bool Application::Initialize(const int& width, const int& height, const char* title, const bool& fullscreen) {
	context = nullptr;

	// initialize GLFW
	if (!glfwInit()) {
		Console::Error << "ERROR : GLFW INITIALISATION FAILED.";
		return false;
	}

	context = new Window(width, height, title, fullscreen);
	inputController.Initialize(context->Get());

	if (!vulkan.Initialize(title, context)) {
		return false;
	}

	Events::EventsManager::GetInstance()->Subscribe("EXIT", &Window::Close, context);
	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POSITION_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &Application::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("SCROLL_INPUT", &Application::OnEvent, this);

	return true;
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

		vulkan.DrawFrame();

		context->SwapBuffers();
		timer.Update();
	}

	vulkan.WaitIdle();
}

void Application::Exit() {
	vulkan.Destroy();

	if (context)
		delete context;

	glfwTerminate();

	Console::Logger::Dump();
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