#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H

#include "../../Input/Events/InputEvents.h"

#include <Entity/Entity.h>

#include <vector>

class CameraObject : public Entity {

	std::vector<Events::KeyInput> keyInputs;

public:

	CameraObject();

	void Initialize() override;

private:

	void Update(const float& dt);

	void KeyHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);

};

#endif