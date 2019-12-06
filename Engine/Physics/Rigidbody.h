<<<<<<< Updated upstream
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Components/Component.h"

class Rigidbody : public Component
{
public:

	float mass;
	bool hasGravity;

	Rigidbody();
	~Rigidbody();

	void Initialize() override;

	void SetActive(const bool& state) override;
};


#endif
=======
#pragma once
class Rigidbody
{
};

>>>>>>> Stashed changes
