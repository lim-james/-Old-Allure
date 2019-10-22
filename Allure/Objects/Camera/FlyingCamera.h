#ifndef FLYING_CAMERA_H
#define FLYING_CAMERA_H

#include "CameraObject.h"

class FlyingCamera : public CameraObject {

	std::map<int, int> keyInputs;

public:

	FlyingCamera();
	~FlyingCamera() override;

	void Build() override;

	void Initialize() override;

private:

	void Update(const float& dt);

	void KeyHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);

};




#endif
