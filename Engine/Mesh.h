#ifndef MESH_H
#define MESH_H

#include "Vertex.h"

#include <vector>

struct Mesh {

	unsigned VAO;
	unsigned indicesSize;

	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);
	~Mesh();

};

#endif