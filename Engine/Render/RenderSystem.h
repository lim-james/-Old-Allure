#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../Systems/System.h"

#include "Framebuffer/Framebuffer.h"
#include "Renderer/FBORenderer.h"
#include "Renderer/AdditiveRenderer.h"

#include "Render.h"
#include "../Components/Camera/Camera.h"
#include "Light/Light.h"

#include "../Events/Event.h"

#include <vector>
#include <map>

typedef std::map<Mesh*, std::vector<mat4f>> MeshBatch;
typedef std::map<Material::Base*, MeshBatch> MaterialBatch;
typedef std::map<Shader*, MaterialBatch> ShaderBatch;

class RenderSystem : public System {

	static const unsigned MAX_LIGHTS = 2;

	bool first = true;

	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
	std::vector<Render*> components;

	unsigned instanceBuffer;
	ShaderBatch batches;

	Shader* depthShader;
	Framebuffer* depthFBO[MAX_LIGHTS];
	mat4f lightSpaceMatrices[MAX_LIGHTS];

	Framebuffer* mainFBO;
	Framebuffer *blurPass, *finalBloomPass;

	Renderer::FBO fboRenderer;
	Renderer::FBO* depthRenderer;
	Renderer::FBO* blurRenderer;
	Renderer::FBO* posterizeRenderer;
	Renderer::Additive* bloomRenderer;

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

	void Batch();
	void RenderWorld();

	void SetLightUniforms(Camera * const camera, Shader * const shader);

};

#endif