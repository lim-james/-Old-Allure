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

	int nocapFps;
	bool fast;
	bool fCull;
	int indicesCount;
	std::string debugText;

public:

	GameScene();

	void Awake() override;
	void Reset() override;
	void Start() override;

	void Update(const float& dt) override;
	void FixedUpdate(const float& dt) override;

	void Destroy() override;

private:

	void MouseHandler(Events::Event* event);
	void KeyHandler(Events::Event* event);
	
	void IndicesHandler(Events::Event* event);

};

#endif
