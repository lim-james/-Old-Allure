#ifndef WINDOW_H
#define WINDOW_H

#include "../Math/Vectors.hpp"
#include "../Events/Event.h"

struct GLFWwindow;

class Window {

	vec2f size;

	GLFWwindow* window;

public:
		
	Window();
	Window(const int& width, const int& height, const char* title, const bool& fullscreen);
	~Window();

	GLFWwindow* Get() const;

	void MakeCurrent() const;

	void SetTitle(const char* title) const;

	const vec2f& GetSize() const;

	template<typename T>
	const Math::vec2<T> GetSize() const {
		return Math::vec2<T>(static_cast<T>(size.x), static_cast<T>(size.y));
	}

	void Close() const;
	bool ShouldClose() const;
	void SwapBuffers() const;

private:

	static void Resize(GLFWwindow* window, int width, int height);

	void OnEvent(Events::Event* event);

};

#endif