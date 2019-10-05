#ifndef RENDER_H
#define RENDER_H

#include "../Components/Component.h"

#include "Model/Model.h"

struct Render : Component {

	Model* model;

	Render();
	~Render() override;

	void Initialize() override;

	void SetActive(const bool& state) override;

};

#endif