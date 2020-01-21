#include "TexturedMaterial.h"

#include <GL/glew.h>

Material::Textured::Textured() : texture(0) {
	lit = false;
	shader = new Shader("Files/Shaders/nonlit.vert", "Files/Shaders/textured.frag");
}

void Material::Textured::SetAttributes() {
	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}