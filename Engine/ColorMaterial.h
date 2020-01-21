#ifndef COLOR_MATERIAL_H
#define COLOR_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.hpp>

namespace Material {
	struct Color : Base {

		vec4f color;

		Color();

		void SetAttributes() override;

	};
}

#endif