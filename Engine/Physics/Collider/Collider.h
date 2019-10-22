#ifndef COLLIDER_H
#define COLLIDER_H

#include "../../Components/Component.h"
#include "../Rigidbody.h"

#include "Math/Vectors.hpp"

class Collider : public Component
{
public:
	Rigidbody attachedRigidbody;

	Collider();
	~Collider();
};

#endif