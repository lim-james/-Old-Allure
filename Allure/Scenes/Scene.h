#ifndef SCENE_H
#define SCENE_H

class Scene {

public:

	// Initialization
	virtual void Awake();
	virtual void Reset(); 
	virtual void Start();
	
	// updates every fixed timestep
	virtual void FixedUpdate();
	// updates every frame
	virtual void Update();
	
	virtual void Destroy();

};

#endif