#ifndef FONT_H
#define FONT_H

#include "Character.h"
#include "Mesh.h"

#include <string>
#include <map>

struct Font {

	std::string name;
	vec2f scale;
	// relative to base height
	float lineHeight;

	unsigned count;
	std::map<int, Character> characters;

	unsigned texture;
	
	Mesh* mesh;

	~Font();

};

#endif
