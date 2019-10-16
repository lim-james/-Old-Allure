#ifndef SCENE_H
#define SCENE_H

#include <Entity/EntityManager.h>
#include <Components/ComponentsManager.h>
#include <Systems/SystemsManager.h>

#include <Render/Material/Standard/StandardMaterial.h>
#include <Render/Material/Nonlit/ColorMaterial.h>
#include <Render/Material/Subtractive/SubtractiveMaterial.h>

class Scene {

	EntityManager* entities;
	ComponentsManager* components;
	SystemsManager* systems;

	Material::Standard* normal;
	Material::Standard* red;
	Material::Standard* green;
	Material::Standard* blue;

public:

	Scene();
	~Scene();

	// Initialization
	virtual void Awake();
	virtual void Reset(); 
	virtual void Start();
	
	// updates every fixed timestep
	virtual void FixedUpdate();
	// updates every frame
	virtual void Update(const float& dt);
	
	virtual void Destroy();

};

#endif