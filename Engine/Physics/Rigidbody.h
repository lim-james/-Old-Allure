#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Components/Component.h"
#include "Math/Vectors.hpp"

class Rigidbody : public Component {
public:

	Rigidbody();
	~Rigidbody() override;

	void Initialize() override;

	void SetActive(const bool& state) override;

	void AddForce(vec3f force);
	void ConstantForce(vec3f force);

	float mass;
	bool hasGravity;
	vec3f velocity;
	vec3f force;
};


#endif