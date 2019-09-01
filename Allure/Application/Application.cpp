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
		return;
	}
	// initialize GLEW
	const GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "ERROR : " << glewGetErrorString(err);
	}
}

void Application::Run() {
	Timer timer;
	timer.Start();


}

void Application::Exit() {

}
