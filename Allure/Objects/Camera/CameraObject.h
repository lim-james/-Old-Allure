#ifndef CAMERA_OBJECT_H
#define CAMERA_OBJECT_H

#include "../../Input/InputEvents.h"

#include <Entity/Entity.h>

#include <map>

class CameraObject : public Entity {

public:

	void Build() override;

};

#endif