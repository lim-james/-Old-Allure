#ifndef MATH_HPP
#define MATH_HPP

#include <vector>

namespace Math {

	const float	PI = 3.1415926535897932384626433832795f;

	template<typename T>
	inline T rad(const T& deg) {
		return deg * PI / 180.f;
	}

	template<typename T>
	inline T deg(const T& rad) {
		return rad * 180.f / PI;
	}

	template<typename T>
	T clamp(const T& a, const T& lowerBound, const T& upperBound) {
		if (a < lowerBound) return lowerBound;
		if (a > upperBound) return upperBound;
		return a;
	}

	template<typename T>
	T abs(const T& a) {
		return a < 0 ? -a : a;
	}

	template<typename T>
	std::vector<T> quadratic(const T& a, const T& b, const T& c) {
		const T& d = b * b - 4 * a * c;

		if (d < 0)
			return {};

		return {
			(-b + sqrt(d)) / (2 * a),
			(-b - sqrt(d)) / (2 * a)
		};
	}

	template<typename T>
	bool inRange(const T& value, const T& min, const T& max) {
		return value >= min && value <= max;
	}

}

#endif