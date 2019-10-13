#include "ColorMaterial.h"

Material::Color::Color() : color(1.0f) {
	lit = false;
	shader = new Shader("Files/Shaders/simple.vert", "Files/Shaders/color.frag");
}

void Material::Color::SetAttributes() {
	shader->Use();

	shader->SetVector4("material.color", color);
}