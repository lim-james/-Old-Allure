#ifndef VERTEX_H
#define VERTEX_H

#include <Math/Vectors.hpp>

struct Vertex {

	vec3f position;
	vec3f normal;
	vec2f texCoords;

	bool operator==(const Vertex& rhs) const;

};

#endif