#ifndef EXHIBITION_SCENE_H
#define EXHIBITION_SCENE_H

#include "Scene.h"

#include <Render/Material/Standard/StandardMaterial.h>

class ExhibitionScene : public Scene {

	Material::Standard* normal;

public:

	ExhibitionScene();

	void Awake() override;
	
	void Update(const float& dt) override;

	void Destroy() override;

};

#endif
