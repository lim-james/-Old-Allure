#ifndef STRESS_SCENE_H
#define STRESS_SCENE_H

#include "Scene.h"

#include <Render/Material/Nonlit/ColorMaterial.h>

class StressScene : public Scene {
	
	Material::Color* normal;

public:

	StressScene();

	void Awake() override;

};

#endif