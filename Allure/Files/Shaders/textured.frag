#version 330 core

out vec4 color;

in VS_OUT {
	vec3 fragmentPosition;
	vec3 normal;
	vec2 texCoord;
} vs_out;

uniform sampler2D tex;

void main() {
	color = texture(tex, vs_out.texCoord);
//	color = texture(tex, vs_out.texCoord);
}
