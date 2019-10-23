#ifndef TEXTURED_MATERIAL_H
#define TEXTURED_MATERIAL_H

#include "../Material.h"

namespace Material {
	struct Textured : Base {

		unsigned texture;

		Textured();

		void SetAttributes() override;

	};
}

#endif
