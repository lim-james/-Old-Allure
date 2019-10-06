#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

namespace Material {
	struct Base {
		Shader* shader;

		virtual ~Base();

		virtual void Use() = 0;
	};
}


#endif