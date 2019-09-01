#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Math {

	inline float randValue() {
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	}
	
	template<typename T>
	T randMinMax(const T& min, const T& max) {
		float v = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		T range = max - min;
		return v * range + min;
	}

	inline int randSign() {
		return rand() % 2 * 2 - 1;
	}

}

#endif