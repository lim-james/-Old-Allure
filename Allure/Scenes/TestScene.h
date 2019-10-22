#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "Scene.h"

#include <Render/Material/Standard/StandardMaterial.h>

class TestScene : public Scene {

	Material::Standard* normal;
	Material::Standard* red;
	Material::Standard* green;
	Material::Standard* blue;

public:

	TestScene();

	void Awake() override;

	void Destroy() override;

};

#endif