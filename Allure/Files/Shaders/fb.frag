#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;

void main() {
	color = vec4(vec3(texture(tex, texCoord)), 1.f);
}