#ifndef COLLIDER_H
#define COLLIDER_H

#include "../../Components/Component.h"
#include "../Rigidbody.h"

#include "Math/Vectors.hpp"
#include "CollisionData.h"

class Collider : public Component
{
public:

	struct Bounds {
		vec3f topLeft;
		vec3f topRight;
		vec3f btmLeft;
		vec3f btmRight;

		void SetBounds(vec3f _topLeft, vec3f _topRight, vec3f _btmLeft, vec3f _btmRight) {
			topLeft = _topLeft;
			topRight = _topRight;
			btmLeft = _btmLeft;
			btmRight = _btmRight;
		}

		void SetBounds(vec3f pos, vec3f scale) {
			topLeft = vec3f(pos.x - scale.x / 2, 0, pos.z + scale.z / 2);
			topRight = vec3f(pos.x + scale.x / 2, 0, pos.z + scale.z / 2);
			btmLeft = vec3f(pos.x - scale.x / 2, 0, pos.z - scale.z / 2);
			btmRight = vec3f(pos.x + scale.x / 2, 0, pos.z - scale.z / 2);
		}

		bool WithinBounds(vec3f pos) {
			if (pos.x >= topLeft.x && pos.z <= topLeft.z &&
				pos.x <= topRight.x && pos.z <= topRight.z &&
				pos.x >= btmLeft.x && pos.z >= btmLeft.z &&
				pos.x <= btmRight.x && pos.z >= btmRight.z) {
				return true;
			}
			return false;
		}
	};

	Rigidbody attachedRigidbody;
	bool isEnabled;
	bool isTrigger;
	CollisionData *data;
	vec3f normal;
	Bounds *bounds;

	Collider();
	~Collider();

	void Initialize() override;
	void SetActive(const bool& state) override;

	void OnCollisionEnter(Entity* collided);
	void OnCollisionStay(Entity* collided);
};

#endif