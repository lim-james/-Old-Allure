#include "AdditiveRenderer.h"

Renderer::Additive::Additive() {
	delete shader;

	shader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/additive.frag");
	shader->Use();
	shader->SetInt("tex0", 0);
	shader->SetInt("tex1", 1);
}

void Renderer::Additive::Render(const unsigned& t0, const unsigned& t1) {
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

