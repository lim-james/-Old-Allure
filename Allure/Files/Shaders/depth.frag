#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;

uniform float near;
uniform float far;

float linearDepth(float depth) {
	float z = depth * 2.f - 1.f;
	return (2.f * near * far) / (far + near - z * (far - near));
}

void main() {
	float depth = texture(tex, texCoord).r;// linearDepth(texture(tex, texCoord).r) / far;
	color = vec4(vec3(1.f - depth), 1.f);
}