#include "Window.h"

#include <GLFW/glfw3.h>

Window::Window() {
	size.Set(0.f);
}

Window::Window(const int& width, const int& height, const char* title, const bool& fullscreen) {
	size.Set(static_cast<float>(width), static_cast<float>(height));

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetCursorPos(window, 0.0, 0.0);

	glfwSetWindowSizeCallback(window, Window::Resize);
}

Window::~Window() {
	glfwDestroyWindow(window);
	window = nullptr;
}
	
GLFWwindow* Window::Get() const {
	return window;
}

void Window::MakeCurrent() const {
	glfwMakeContextCurrent(window);
}

void Window::SetTitle(const char* title) const {
	glfwSetWindowTitle(window, title);
}

void Window::Close() const {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(window);
}
	
void Window::SwapBuffers() const {
	glfwSwapBuffers(window);
}

void Window::Resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
