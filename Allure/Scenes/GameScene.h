#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"

#include <Render/Material/Standard/StandardMaterial.h>

class GameScene : public Scene {

	Material::Standard* normal;
	Material::Standard* field;

public:

	GameScene();

	void Awake() override;
	void Start() override;

	void Destroy() override;

};

#endif
