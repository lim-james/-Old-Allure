#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

namespace Material {
	struct Base {
		bool lit;

		Shader* shader;

		Base();
		virtual ~Base();

		virtual void Use() = 0;
	};
}


#endif