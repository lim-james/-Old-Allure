#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in mat4 iModel;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	vec4 position = vec4(inPosition, 1.f);
	gl_Position = projection * view * model * position;
}