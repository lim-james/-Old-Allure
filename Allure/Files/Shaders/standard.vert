#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in vec4 iColor;
layout(location = 4) in mat4 iModel;

out VS_OUT {
	vec3 fragmentPosition;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPosLightSpace;
	vec4 color;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main() {
	vec4 position = vec4(inPosition, 1.f);

	gl_Position = projection * view * model * position;

	vs_out.fragmentPosition = vec3(model * position);
	vs_out.normal = mat3(transpose(model)) * inNormal;

	vs_out.texCoord = inTexCoord;
	vs_out.fragPosLightSpace = lightSpaceMatrix * vec4(vs_out.fragmentPosition, 1.f);
	vs_out.color = iColor;
}