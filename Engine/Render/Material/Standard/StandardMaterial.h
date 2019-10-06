#ifndef STANDARD_MATERIAL_H
#define STANDARD_MATERIAL_H

#include "../Material.h"

#include <Math/Vectors.hpp>

namespace Material {
	
	struct Standard : Base {
		
		unsigned albedo;
		vec3f tint;
		
		unsigned metallic;
		float smoothness;

		Standard();

		void Use() override;

	};

}

#endif