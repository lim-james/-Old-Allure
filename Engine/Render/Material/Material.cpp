#include "Material.h"

Material::Base::Base() 
	: lit(false)
	, shader(nullptr) {}

Material::Base::~Base() {
	delete shader;
}
