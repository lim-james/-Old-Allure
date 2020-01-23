#ifndef OUTLINE_MATERIAL_H
#define OUTLINE_MATERIAL_H

#include "Material.h"

#include <Math/Vectors.hpp>

namespace Material {
	
	struct Outline : Base {
		
		unsigned albedo;
		vec3f tint;
		vec3f outlineTint;
		float et;
		
		unsigned metallic;
		float smoothness;

		Outline();

		void SetAttributes() override;

	};

}

#endif
