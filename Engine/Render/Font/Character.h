#ifndef CHARACTER_H
#define CHARACTER_H

#include <Math/Vectors.hpp>

struct Character {
	unsigned index;
	vec4i rect;
	vec2i offset;
	int xAdvance;
};

#endif
