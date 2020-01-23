#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "System.h"

#include "Framebuffer.h"
#include "FBORenderer.h"
#include "AdditiveRenderer.h"

#include "Render.h"
#include "Camera.h"
#include "Light.h"
#include "Text.h"`

#include <Events/Event.h>
#include <Containers/Quad.hpp>

#include <vector>
#include <map>

typedef std::map<Mesh*, std::vector<mat4f>> MeshBatch;
typedef std::map<Material::Base*, MeshBatch> MaterialBatch;
typedef std::map<Shader*, MaterialBatch> ShaderBatch;
typedef std::map<std::string, ShaderBatch> Batches;
typedef std::map<Font*, std::vector<Text*>> TextBatches;

class RenderSystem : public System {

	static const unsigned MAX_LIGHTS = 8;

	bool first = true;

	//Model* quad;
	//std::vector<mat4f> quadBatch;

	vec2i windowSize;
	std::vector<Camera*> cameras;
	std::vector<Light*> lights;
	std::vector<Render*> components;

	unsigned instanceBuffer;
	Batches batches;
	TextBatches textBatches;

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
	bool LOD;

	int indicesCount;
	int cullCount;

public:

	//RenderSystem();
	~RenderSystem() override;
	
	void Initialize() override;
	void Update(const float& t) override;

private:

	// components handlers

	// Camera
	void CameraActiveHandler(Events::Event* event);
	void CameraDepthHandler(Events::Event* event);
	// Light
	void LightActiveHandler(Events::Event* event);
	// Render
	void RenderActiveHandler(Events::Event* event);
	// Text
	void TextActiveHandler(Events::Event* event);
	void TextFontHandler(Events::Event* event);

	// events handlers

	void ResizeHandle(Events::Event* event);

	void DebugTextHandler(Events::Event* event);
	void FrustrumCullHandler(Events::Event* event);
	void PartitionHandler(Events::Event* event);
	void LODHandler(Events::Event* event);

	// helper methods

	void Batch();
	//void Traverse(Quad<Entity*> * const quad, const vec3f& pos, const vec3f& dir, const float& theta);

	void SetLightUniforms(const vec3f& viewPosition, Shader * const shader);
	void RenderWorld(MeshBatch& batch);
	void RenderText(TextBatches& textBatches);

};

#endif