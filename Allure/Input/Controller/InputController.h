#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include "../../Math/Vectors.hpp"
#include "../../Events/Event.h"

struct GLFWwindow;

class InputController {

	vec2f cursorPosition;

public:

	void Initialize(GLFWwindow* context);

private:

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void CharCallback(GLFWwindow* window, unsigned codepoint);
	static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
	static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

	void OnEvent(Events::Event* event);

};

#endif