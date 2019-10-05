#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../Systems/System.h"
#include "Render.h"

#include "../Components/Camera/Camera.h"
#include "Shader/Shader.h"

#include "../Events/Event.h"

class RenderSystem : public System {

	Shader* shader;

	std::vector<Camera*> cameras;
	std::vector<Render*> components;

public:

	RenderSystem();
	~RenderSystem() override;

	void Update(const float& t) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void RenderActiveHandler(Events::Event* event);

};

#endif