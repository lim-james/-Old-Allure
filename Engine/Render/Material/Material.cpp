#include "Material.h"

Material::Base::Base() 
	: lit(false)
	, shader(nullptr) {}

Material::Base::~Base() {
	delete shader;
}

bool Material::Base::IsLit() const {
	return lit;
}

Shader * const Material::Base::GetShader() const {
	return shader;
}
