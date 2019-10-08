#version 330 core

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform mat4 model;

void main() {
	gl_Position = model * vec4(inPosition, 0.f, 1.f);
	texCoord = inTexCoord;
}