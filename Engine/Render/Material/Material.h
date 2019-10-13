#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

namespace Material {

	struct Base {

		Base();
		virtual ~Base();

		bool IsLit() const;
		Shader * const GetShader() const;

		virtual void SetAttributes() = 0;

	protected:

		bool lit;
		Shader* shader;

	};

}


#endif