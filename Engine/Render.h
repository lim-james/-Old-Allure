#ifndef RENDER_H
#define RENDER_H

#include "Component.h"

#include "Material.h"
#include "Model.h"

#include <string>

struct Render : Component {

	Material::Base* material;

	Model* model;
	// LOD
	Model* midModel;
	Model* lowModel;

	std::vector<std::string> groups;

	Render();
	~Render() override;

	void Initialize() override;

	void SetActive(const bool& state) override;

};

#endif