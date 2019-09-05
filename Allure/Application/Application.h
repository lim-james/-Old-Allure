#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Events/Event.h"

#include "../Window/Window.h"
#include "../Input/Controller/InputController.h"

#include "../Vulkan/Vulkan.h"

#include <vector>

class Application {

	const unsigned FPS = 60;
	const unsigned FRAMERATE = 1000 / FPS;

	Window* context;
	InputController inputController;

	Vulkan vulkan;

public:

	bool Initialize(const int& width, const int& height, const char* title, const bool& fullscreen);
	void Run();
	void Exit();
	
private:

	void OnEvent(Events::Event* event);

};

#endif
