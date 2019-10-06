#ifndef RENDER_H
#define RENDER_H

#include "../Components/Component.h"

#include "Material/Material.h"
#include "Model/Model.h"

struct Render : Component {

	Material::Base* material;
	Model* model;

	Render();
	~Render() override;

	void Initialize() override;

	void SetActive(const bool& state) override;

};

#endif