#ifndef RENDER_H
#define RENDER_H

#include "../Component.h"

struct Render : public Component {

	Render();
	~Render() override;

	void Initialize() override;

	void SetActive(const bool& state) override;

};

#endif