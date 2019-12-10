#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "../Systems/System.h"

#include "Framebuffer/Framebuffer.h"
#include "Renderer/FBORenderer.h"
#include "Renderer/AdditiveRenderer.h"

#include "Render.h"
#include "../Components/Camera/Camera.h"
#include "Light/Light.h"

#include <Events/Event.h>
#include <Quad.hpp>

#include <vector>
#include <map>

typedef std::map<Mesh*, std::vector<mat4f>> MeshBatch;
typedef std::map<Material::Base*, MeshBatch> MaterialBatch;
typedef std::map<Shader*, MaterialBatch> ShaderBatch;

class RenderSystem : public System {

	static const unsigned MAX_LIGHTS = 8;

	bool first = true;

	Model* quad;
	std::vector<mat4f> quadBatch;

	vec2i windowSize;
	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
	std::vector<Render*> components;

	Camera* canvas;
	mat4f canvasLookAt;
	Shader* uiShader;

	unsigned instanceBuffer;
	ShaderBatch batches;

	Shader* depthShader;
	Framebuffer* depthFBO[MAX_LIGHTS];
	mat4f lightSpaceMatrices[MAX_LIGHTS];

	Framebuffer* mainFBO;
	Framebuffer *blurPass, *finalBloomPass;

	Shader* textShader;

	Renderer::FBO fboRenderer;
	Renderer::FBO* depthRenderer;
	Renderer::FBO* blurRenderer;
	Renderer::FBO* posterizeRenderer;
	Renderer::Additive* bloomRenderer;

	std::string debugText;
	bool frustrumCull;
	bool partition;

	int indicesCount;
	int cullCount;

public:

	RenderSystem();
	~RenderSystem() override;

	void Start() override;

	void Update(const float& t) override;
	void FixedUpdate(const float& t) override;

private:

	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);

	void LightActiveHandler(Events::Event* event);

	void RenderActiveHandler(Events::Event* event);

	void ResizeHandle(Events::Event* event);

	void DebugTextHandler(Events::Event* event);
	void FrustrumCullHandler(Events::Event* event);
	void PartitionHandler(Events::Event* event);

	void Batch();
	void Traverse(Quad<Entity*> * const quad, const vec3f& pos, const vec3f& dir, const float& theta);

	void RenderWorld();

	void SetLightUniforms(Camera * const camera, Shader * const shader);

};

#endif