#version 330 core

out vec4 color;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	int type;

	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float power;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

in VS_OUT {
	vec3 fragmentPosition;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPosLightSpace;
	vec4 color;
} vs_out;

uniform float near;
uniform float far;

uniform vec3 viewPosition;
uniform Material material;

uniform Light lights[16];
uniform int lightCount;

float linearDepth(float depth) {
	float z = depth * 2.f - 1.f;
	return (2.f * near * far) / (far + near - z * (far - near));
}

vec3 calcDirectionalLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= -normalize(light.direction);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = max(dot(normal, lightDirection), 0);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0), material.shininess);
	
	vec3 ambient	= light.ambient * materialPoint;
	vec3 diffuse	= diff * light.diffuse * materialPoint;
	vec3 specular	= spec * light.specular * specularPoint;

	return ambient + diffuse + specular;
}

vec3 calcPointLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= normalize(light.position - vs_out.fragmentPosition);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = max(dot(normal, lightDirection), 0);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0), material.shininess);
		
	float dist			= length(light.position - vs_out.fragmentPosition);
	float attenuation	= light.power / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient	= attenuation * light.ambient * materialPoint;
	vec3 diffuse	= attenuation * diff * light.diffuse * materialPoint;
	vec3 specular	= attenuation * spec * light.specular * specularPoint;
	
	return ambient + diffuse + specular;
}

vec3 calcSpotLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= -normalize(light.position - vs_out.fragmentPosition);
	vec3 reflectDirection	= reflect(lightDirection, normal);

	float diff	= max(dot(normal, -lightDirection), 0);
	float spec	= pow(max(dot(viewDirection, reflectDirection), 0), material.shininess);
		
	float theta		= dot(lightDirection, normalize(light.direction));
	float epsilon	= light.cutOff - light.outerCutOff;
	float intensity	= clamp((theta - light.outerCutOff) / epsilon, 0, 1.f);

	float dist			= length(light.position - vs_out.fragmentPosition);
	float attenuation	= light.power / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient	= attenuation * light.ambient * materialPoint;
	vec3 diffuse	= attenuation * intensity * diff * light.diffuse * materialPoint;
	vec3 specular	= attenuation * intensity * spec * light.specular * specularPoint;

	return ambient + diffuse + specular;
}

void main() {
	vec3 viewDirection = normalize(viewPosition - vs_out.fragmentPosition);

	vec4 diffuse = vec4(1.0f);// texture(material.diffuse, vs_out.texCoord) * vs_out.color;
	vec3 materialPoint = diffuse.rgb;
	vec3 specularPoint = vec3(0.0f); //texture(material.specular, vs_out.texCoord).rgb;

	vec3 add = vec3(0.f);
	vec3 result = vec3(0.f);

	vec3 normal = normalize(vs_out.normal);

	for (int i = 0; i < lightCount; ++i) {
		if (lights[i].type == 0) {
			result += calcPointLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else if (lights[i].type == 1) {
			result += calcDirectionalLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else {
			result += calcSpotLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		}
	}

	color = vec4(result, 1.0f);// diffuse.a);
}