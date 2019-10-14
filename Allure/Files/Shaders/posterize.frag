#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex;

float sround(float value, float segments) {
	return round(value * segments) / segments;
}

void main() {
//	float size = 128.0f;
//	vec2 resultCoord = vec2(ivec2(texCoord * size)) / size + 0.5f / size;
//	vec3 result = texture(tex, resultCoord).rgb;
//	color = vec4(result, 1.f);
	color = texture(tex, texCoord);
	float range = 2.0f;
	color.r = sround(color.r, range);
	color.g = sround(color.g, range);
	color.b = sround(color.b, range);
}
