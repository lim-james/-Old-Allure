#ifndef WINDOW_H
#define WINDOW_H

#include <Events/Event.h>
#include <Math/Vectors.hpp>

struct GLFWwindow;

class Window {

	vec2i size;

	GLFWwindow* window;

public:
		
	Window();
	Window(const int& width, const int& height, const char* title, const bool& fullscreen);
	~Window();

	GLFWwindow* Get() const;

	void MakeCurrent() const;

	void SetTitle(const char* title) const;

	void Close() const;
	bool ShouldClose() const;
	void SwapBuffers() const;

	void BroadcastSize() const;

private:

	static void Resize(GLFWwindow* window, int width, int height);

	void OnEvent(Events::Event* event);

};

#endif