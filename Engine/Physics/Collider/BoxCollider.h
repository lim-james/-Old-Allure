#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

class BoxCollider : public Collider
{
public:

	vec3f position;
	vec3f size;

	BoxCollider();
	~BoxCollider();


	void Initialize();

	void SetActive(const bool& state);

};

#endif
