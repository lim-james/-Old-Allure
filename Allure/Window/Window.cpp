#include "Window.h"

#include "../Events/Manager/EventsManager.h"	

#include "../Logger/Logger.h"

#include <GLFW/glfw3.h>

Window::Window() 
	: size(0.f)
	, resize(false)
	, window(nullptr) {}

Window::Window(const int& width, const int& height, const char* title, const bool& fullscreen) {
	size.Set(static_cast<float>(width), static_cast<float>(height));

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetCursorPos(window, 0.0, 0.0);

	glfwSetWindowSizeCallback(window, Window::Resize);

	Events::EventsManager::GetInstance()->Subscribe("WINDOW_RESIZE", &Window::OnEvent, this);
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

const vec2f& Window::GetSize() const {
	return size;
}

void Window::FetchSize() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	size.Set(static_cast<float>(width), static_cast<float>(height));
}

const bool Window::DidResize() const {
	return resize;
}

void Window::ResetResize() {
	resize = false;
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
	const vec2f size(static_cast<float>(width), static_cast<float>(height));
	Events::EventsManager::GetInstance()->Trigger("WINDOW_RESIZE", new Events::AnyType<vec2f>(size));
}

void Window::OnEvent(Events::Event* event) {
	if (event->name == "WINDOW_RESIZE") {
		Events::AnyType<vec2f>* newSize = static_cast<Events::AnyType<vec2f>*>(event);
		size = newSize->data;
		resize = true;
		//Console::Log << "SIZE : " << size << '\n';
	}
}
