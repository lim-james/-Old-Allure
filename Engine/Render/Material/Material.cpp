#include "Material.h"

Material::Base::~Base() {
	delete shader;
}
