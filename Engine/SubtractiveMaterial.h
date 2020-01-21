#ifndef SUBTRACTIVE_MATERIAL_H
#define SUBTRACTIVE_MATERIAL_H

#include "StandardMaterial.h"

namespace Material {
	struct Subtractive : Standard {
		void SetAttributes() override;
	};
}

#endif