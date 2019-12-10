#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider
{
public:

	float center;
	float radius;

	SphereCollider();
	~SphereCollider();

};

#endif