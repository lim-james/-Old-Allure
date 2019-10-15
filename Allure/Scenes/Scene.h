#ifndef SCENE_H
#define SCENE_H

#include <Entity/EntityManager.h>
#include <Components/ComponentsManager.h>
#include <Systems/SystemsManager.h>

#include <Render/Material/Standard/StandardMaterial.h>
#include <Render/Material/Nonlit/ColorMaterial.h>

class Scene {

	EntityManager* entities;
	ComponentsManager* components;
	SystemsManager* systems;

	Material::Standard* container;
	Material::Standard* bulb;

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