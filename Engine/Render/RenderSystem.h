#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../Systems/System.h"

#include "Framebuffer/Framebuffer.h"
#include "Renderer/FBORenderer.h"

#include "Render.h"
#include "../Components/Camera/Camera.h"
#include "Light/Light.h"

#include "../Events/Event.h"

#include <vector>

class RenderSystem : public System {

	static const unsigned MAX_LIGHTS = 16;

	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
	std::vector<Render*> components;

	Shader* depthShader;
	Framebuffer* depthFBO;

	Renderer::FBO fboRenderer;

public:

	RenderSystem();
	~RenderSystem() override;

	void Update(const float& t) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void LightActiveHandler(Events::Event* event);

	void RenderActiveHandler(Events::Event* event);

	void ResizeHandle(Events::Event* event);

	void SetLightUniforms(Camera * const camera, Shader * const shader, Light * const light);

};

#endif