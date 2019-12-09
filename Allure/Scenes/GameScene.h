#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"

#include "../Objects/GameObject/GameObject.h"
#include "../Objects/Camera/FlyingCamera.h"

#include <Render/Material/Standard/StandardMaterial.h>

class GameScene : public Scene {

	Material::Standard* normal;
	Material::Standard* field;

	FlyingCamera* camera;
	GameObject* ball;

public:

	GameScene();

	void Awake() override;
	void Reset() override;
	void Start() override;

	void Destroy() override;

private:

	void MouseHandler(Events::Event* event);

};

#endif
