#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../System.h"

#include "../../Components/Render/Render.h"
#include "../../Events/Event.h"

#include <set>

class RenderSystem : public System {

	std::set<Render*> components;

public:

	RenderSystem();
	~RenderSystem() override;

	void Update(const float& t) override;

private:

	void ActiveHandler(Events::Event* event);

};

#endif