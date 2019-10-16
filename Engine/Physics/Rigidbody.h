#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../Components/Component.h"

class Rigidbody : public Component
{
public:
	Rigidbody();
	~Rigidbody();

	void Initialize() override;

	void SetActive(const bool& state) override;
};


#endif