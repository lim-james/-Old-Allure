#include "StandardMaterial.h"

#include "../../Load/LoadTGA.h"

#include <GL/glew.h>

Material::Standard::Standard() 
	: tint(1.f)
	, smoothness(32.f) {
	lit = true;

	shader = new Shader("Files/Shaders/standard.vert", "Files/Shaders/standard.frag");
	shader->Use();
	shader->SetInt("material.albedo", 0);
	shader->SetInt("material.metallic", 1);
	for (unsigned i = 0; i < 16; ++i) {
		shader->SetInt("lights[" + std::to_string(i) + "].shadowMap", 2 + i);
	}

	albedo = Load::TGA("Files/Textures/white.tga");
	metallic = Load::TGA("Files/Textures/white.tga");
}

void Material::Standard::SetAttributes() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);

	shader->SetVector3("material.tint", tint);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metallic);

	shader->SetFloat("material.smoothness", smoothness);
}