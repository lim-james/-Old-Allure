#include "StandardMaterial.h"

Material::Standard::Standard() {
	shader = new Shader("Files/Shaders/standard.vert", "Files/Shaders/standard.frag");
}

void Material::Standard::Use() {
	shader->Use();
	//shader.SetInt("material.albedo", albedo);
}