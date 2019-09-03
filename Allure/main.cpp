#include "Application/Application.h"

int main() {

	Application app;

	if (app.Initialize(1600, 900, "", false)) {
		app.Run();
	}

	app.Exit();

	return 0;
}