#ifndef APPLICATION_H
#define APPLICATION_H

#include "../Window/Window.h"
#include "../Input/Controller/InputController.h"

#include <Events/Event.h>

class Application {

	const unsigned FPS = 60;
	const unsigned FRAMERATE = 1000 / FPS;

	Window* context;
	InputController inputController;

public:

	void Initialize(const int& width, const int& height, const char* title, const bool& fullscreen);
	void Run();
	void Exit();
	
private:

	void OnEvent(Events::Event* event);

};

#endif
