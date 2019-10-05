#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H

#include "../../Input/InputEvents.h"

#include <Entity/Entity.h>

#include <map>

class CameraObject : public Entity {

	std::map<int, int> keyInputs;

public:

	CameraObject();
	~CameraObject() override;

	void Initialize() override;

private:

	void Update(const float& dt);

	void KeyHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);

};

#endif