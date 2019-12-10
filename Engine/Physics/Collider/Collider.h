#ifndef COLLIDER_H
#define COLLIDER_H

#include "../../Components/Component.h"
#include "../Rigidbody.h"

#include "Math/Vectors.hpp"
#include "CollisionData.h"

class Collider : public Component
{
public:



	Rigidbody attachedRigidbody;
	bool isEnabled;
	bool isTrigger;
	CollisionData *data;

	Collider();
	~Collider();

	void Initialize() override;
	void SetActive(const bool& state) override;

	void OnCollisionEnter(Entity* collided);
	void OnCollisionStay(Entity* collided);
};

#endif