<<<<<<< Updated upstream
#pragma once
class SphereCollider
{
};

=======
#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider
{
public:

	vec3f position;
	vec3f size;

	SphereCollider();
	~SphereCollider();

	void Initialize();

	void SetActive(const bool& state);
};

#endif
>>>>>>> Stashed changes
