#ifndef FONT_H
#define FONT_H

#include "Character.h"
#include "../Model/Mesh.h"

#include <string>
#include <map>

struct Font {

	std::string name;
	unsigned lineHeight;
	unsigned base;
	vec2u scale;

	unsigned count;
	std::map<int, Character> characters;

	unsigned texture;
	
	Mesh* mesh;

	~Font();

};

#endif
