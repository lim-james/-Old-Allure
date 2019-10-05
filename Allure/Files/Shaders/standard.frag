#version 330 core

out vec4 color;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

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
	vec3 color;

	float power;

	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;	
	
	sampler2D shadowMap;
};

struct Fog {
	vec3 color;
	float start;
	float end;
	float density;
	int type;
	bool enabled;
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
uniform Fog fog;

uniform int lightCount;

uniform vec3 prevPosition;
uniform vec4 prevColor;

float linearDepth(float depth) {
	float z = depth * 2.f - 1.f;
	return (2.f * near * far) / (far + near - z * (far - near));
}

float getFogFactor(Fog fog, float dist) {
	if (!fog.enabled)
		return 0.f;

	float factor = 0.f;

	if (fog.type == 0)
		factor = (fog.end - dist) / (fog.end - fog.start);
	else if (fog.type == 1)
		factor = exp(-fog.density * dist);
	else if (fog.type == 2)
		factor = exp(-pow(fog.density * dist, 2.f));

	factor = 1.f - clamp(factor, 0.f, 1.f);

	return factor;
}

float calculateShadow(vec4 fragPosLightSpace, Light light) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float closestDepth = texture(light.shadowMap, projCoords.xy).r; 

	float bias = max(0.05 * (1.0 - dot(vs_out.normal, -light.direction)), 0.005);  

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(light.shadowMap, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = texture(light.shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 calcDirectionalLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= -normalize(light.direction);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = max(dot(normal, lightDirection), 0);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0), material.shininess);
	
	vec3 ambient	= light.ambient * materialPoint;
	vec3 diffuse	= diff * light.diffuse * materialPoint;
	vec3 specular	= spec * light.specular * specularPoint;

	float shadow = calculateShadow(vs_out.fragPosLightSpace, light);

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
	
	float shadow = calculateShadow(vs_out.fragPosLightSpace, light);

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

	float shadow = calculateShadow(vs_out.fragPosLightSpace, light);
	
	return ambient + diffuse + specular;
	//return ambient + (1.f - shadow) * (diffuse + specular);
}

void main() {
	vec3 viewDirection = normalize(viewPosition - vs_out.fragmentPosition);

	vec4 diffuse = texture(material.diffuse, vs_out.texCoord) * vs_out.color;
	vec3 materialPoint = diffuse.rgb;
	vec3 specularPoint = texture(material.specular, vs_out.texCoord).rgb;

//	if (length(materialPoint) < 0.1) discard;

	vec3 add = vec3(0.f);
	vec3 result = vec3(0.f);

	vec3 normal = normalize(vs_out.normal);

	for (int i = 0; i < lightCount; ++i) {
		if (lights[i].type == 0) {
			add = calcPointLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else if (lights[i].type == 1) {
			add = calcDirectionalLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else {
			add = calcSpotLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		}

		result += add * lights[i].color;
	}

	float depth = linearDepth(gl_FragCoord.z);
	//float fogFactor = getFogFactor(fog, depth);
	//result = mix(result * vs_out.color.rgb, vec3(fogFactor), .5f);
	color = vec4(result, diffuse.a);
}