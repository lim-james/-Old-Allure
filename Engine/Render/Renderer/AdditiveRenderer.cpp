#include "AdditiveRenderer.h"

Renderer::Additive::Additive() {
	delete shader;

	shader = new Shader("Files/Shaders/fb.vert", "Files/Shaders/additive.frag");
	shader->Use();
	shader->SetInt("tex0", 0);
	shader->SetInt("tex1", 1);
}

Renderer::Additive::Additive(const std::string& vPath, const std::string& fPath) {
	delete shader;

	shader = new Shader(vPath, fPath);
	shader->Use();
	shader->SetInt("tex0", 0);
	shader->SetInt("tex1", 1);
}

void Renderer::Additive::Render(const unsigned& t0, const unsigned& t1) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t1);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

