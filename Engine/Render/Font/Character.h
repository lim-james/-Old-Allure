#ifndef CHARACTER_H
#define CHARACTER_H

#include <Math/Vectors.hpp>

// values are relative to base height
struct Character {
	unsigned index;
	vec4f rect;
	float xAdvance;
};

#endif
