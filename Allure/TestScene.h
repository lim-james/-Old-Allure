#ifndef TEST_SCENE_H
#define TEST_SCENE_H

#include "Scene.h"

#include <Transform.h>
#include <Light.h>
#include <StandardMaterial.h>

class TestScene : public Scene {

	float et;

	Material::Standard* normal;
	Material::Standard* red;
	Material::Standard* green;
	Material::Standard* blue;

	vec2f movement;
	float cameraSpeed;
	Transform* cameraTransform;

	Transform* directionalLight;

public:

	void Awake() override;
	void Start() override;

	void Update(const float& dt);
	
	void Destroy() override;

private:

	void KeyInputHandler(Events::Event* event);
	void CursorPositionHandler(Events::Event* event);

	void CreateCube(const vec3f& translation, const vec3f& scale, Material::Base* material);
	void CreateSphere(const vec3f& translation, const vec3f& scale, Material::Base* material);

	Light* CreateDirectionalLight(const vec3f& translation, const vec3f& rotation);
	Light* CreateSpotLight(const vec3f& translation, const vec3f& rotation);

};

#endif