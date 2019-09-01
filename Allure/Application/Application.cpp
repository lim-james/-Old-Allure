#include "Application.h"

#include "../Timer/Timer.h"

// external
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// standard
#include <iostream>


void Application::Initialize(const int& width, const int& height, const char* title, const bool& fullscreen) {
	// initialize GLFW
	if (!glfwInit()) {
		std::cout << "ERROR : GLFW INITIALISATION FAILED.";
		return;
	}

	context = new Window(width, height, title, fullscreen);

	// initialize GLEW
	glewExperimental = true;
	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "ERROR : " << glewGetErrorString(err);
	}
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

		context->SwapBuffers();
		timer.Update();
	}
}

void Application::Exit() {
	delete context;
}
