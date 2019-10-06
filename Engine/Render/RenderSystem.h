#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../Systems/System.h"
#include "Render.h"

#include "../Components/Camera/Camera.h"
#include "Light/Light.h"
#include "Shader/Shader.h"

#include "../Events/Event.h"

#include <vector>

class RenderSystem : public System {

	static const unsigned MAX_LIGHTS = 16;

	Shader* shader;

	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
	std::vector<Render*> components;

public:

	RenderSystem();
	~RenderSystem() override;

	void Update(const float& t) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void LightActiveHandler(Events::Event* event);

	void RenderActiveHandler(Events::Event* event);

	void SetLightUniforms(Shader * const shader);

};

#endif