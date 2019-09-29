#include "InputController.h"

#include "../Events/InputEvents.h"

#include <Events/Manager/EventsManager.h>

#include <GLFW/glfw3.h>

#include <map>

void InputController::Initialize(GLFWwindow* context) {
		

	glfwSetKeyCallback(context, KeyCallback);
	glfwSetCharCallback(context, CharCallback);
	glfwSetCursorPosCallback(context, CursorPosCallback);
	glfwSetMouseButtonCallback(context, MouseButtonCallback);
	glfwSetScrollCallback(context, ScrollCallback);

	Events::EventsManager::GetInstance()->Subscribe("KEY_INPUT", &InputController::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("TEXT_INPUT", &InputController::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("CURSOR_POS_UPDATE", &InputController::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("MOUSE_BUTTON_INPUT", &InputController::OnEvent, this);
	Events::EventsManager::GetInstance()->Subscribe("SCROLL_INPUT", &InputController::OnEvent, this);
}

void InputController::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Events::KeyInput* data = new Events::KeyInput(key, scancode, action, mods);
	Events::EventsManager::GetInstance()->Trigger("KEY_INPUT", data);
}

void InputController::CharCallback(GLFWwindow* window, unsigned codepoint) {
	Events::TextInput* data = new Events::TextInput(static_cast<char>(codepoint));
	Events::EventsManager::GetInstance()->Trigger("TEXT_INPUT", data);
}

void InputController::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
	const vec2f position(static_cast<float>(xpos), static_cast<float>(ypos));
	Events::AnyType<vec2f>* data = new Events::AnyType<vec2f>(position);
	Events::EventsManager::GetInstance()->Trigger("CURSOR_POS_UPDATE", data);
}

void InputController::MouseButtonCallback(GLFWwindow* window, int key, int action, int mods) {
	Events::MouseButtonInput* data = new Events::MouseButtonInput(key, action, mods);
	Events::EventsManager::GetInstance()->Trigger("MOUSE_BUTTON_INPUT", data);
}

void InputController::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	Events::ScrollInput* data = new Events::ScrollInput(
		vec2f(static_cast<float>(xOffset), static_cast<float>(yOffset))
	);
	Events::EventsManager::GetInstance()->Trigger("SCROLL_INPUT", data);
}

void InputController::OnEvent(Events::Event* event) {
	if (event->name == "KEY_INPUT") {
		Events::KeyInput* input = static_cast<Events::KeyInput*>(event);
	} else if (event->name == "TEXT_INPUT") {
		Events::TextInput* input = static_cast<Events::TextInput*>(event);
	} else if (event->name == "TEXT_INPUT") {
	} else if (event->name == "CURSOR_POS_UPDATE") {
		Events::AnyType<vec2f>* update = static_cast<Events::AnyType<vec2f>*>(event);
		const vec2f& position = update->data;
		Events::CursorPositionInput* input = new Events::CursorPositionInput(position, cursorPosition - position);
		Events::EventsManager::GetInstance()->Trigger("CURSOR_POSITION_INPUT", input);
		cursorPosition = position;
	} else if (event->name == "MOUSE_BUTTON_INPUT") {
		Events::MouseButtonInput* input = static_cast<Events::MouseButtonInput*>(event);
	} else if (event->name == "SCROLL_INPUT") {
		Events::ScrollInput* input = static_cast<Events::ScrollInput*>(event);
	}

}

