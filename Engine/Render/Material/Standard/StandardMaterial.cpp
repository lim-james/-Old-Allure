#include "StandardMaterial.h"

#include "../../Load/LoadTGA.h"

#include <GL/glew.h>

Material::Standard::Standard() 
	: tint(1.f)
	, smoothness(0.5f) {
	lit = true;

	shader = new Shader("Files/Shaders/standard.vert", "Files/Shaders/standard.frag");
	shader->Use();
	shader->SetInt("material.albedo", 0);
	shader->SetInt("material.metallic", 1);
	shader->SetInt("shadowMap", 2);

	albedo = Load::TGA("Files/Textures/white.tga");
	metallic = Load::TGA("Files/Textures/white.tga");
}

void Material::Standard::Use() {
	shader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);

	shader->SetVector3("material.tint", tint);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, metallic);

	shader->SetFloat("material.smoothness", smoothness);
}