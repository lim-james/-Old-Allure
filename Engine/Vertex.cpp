#include "Vertex.h"

#include <tuple>

bool Vertex::operator==(const Vertex& rhs) const {
	return position == rhs.position
		&& texCoords == rhs.texCoords
		&& normal == rhs.normal;
}