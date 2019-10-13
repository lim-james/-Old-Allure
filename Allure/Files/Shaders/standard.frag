#version 330 core

out vec4 color;

struct Material {
	sampler2D albedo;
	vec3 tint;

	sampler2D metallic;
	float smoothness;
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

	sampler2D shadowMap;
};

struct LightSpacePoints {
	vec4 positions[16];
};

in VS_OUT {
	vec3 fragmentPosition;
	vec3 normal;
	vec2 texCoord;
	LightSpacePoints fragPosLightSpace;
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

vec3 calcDirectionalLight(vec4 fragPosLightSpace, Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= normalize(-light.direction);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = max(dot(normal, lightDirection), 0);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0), material.smoothness);
	
	vec3 ambient	= light.ambient * materialPoint;
	vec3 diffuse	= diff * light.diffuse * materialPoint;
	vec3 specular	= spec * light.specular * specularPoint;

	float shadow = calculateShadow(fragPosLightSpace, light);

//	return ambient + diffuse + specular;
	return ambient + (1.0f - shadow) * (diffuse + specular);
}

vec3 calcPointLight(Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= normalize(light.position - vs_out.fragmentPosition);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff = max(dot(normal, lightDirection), 0);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0), material.smoothness);
	
	float dist			= length(light.position - vs_out.fragmentPosition);
	float attenuation	= light.power / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient	= attenuation * light.ambient * materialPoint;
	vec3 diffuse	= attenuation * diff * light.diffuse * materialPoint;
	vec3 specular	= attenuation * spec * light.specular * specularPoint;
	
	return ambient + diffuse + specular;
}

vec3 calcSpotLight(vec4 fragPosLightSpace, Light light, vec3 normal, vec3 viewDirection, vec3 materialPoint, vec3 specularPoint) {
	vec3 lightDirection		= normalize(light.position - vs_out.fragmentPosition);
	vec3 reflectDirection	= reflect(-lightDirection, normal);

	float diff	= max(dot(normal, lightDirection), 0);
	float spec	= pow(max(dot(viewDirection, reflectDirection), 0), material.smoothness);
		
	float theta		= dot(lightDirection, normalize(-light.direction));
	float epsilon	= light.cutOff - light.outerCutOff;
	float intensity	= clamp((theta - light.outerCutOff) / epsilon, 0, 1.f);

	float dist			= length(light.position - vs_out.fragmentPosition);
	float attenuation	= light.power / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient	= attenuation * light.ambient * materialPoint;
	vec3 diffuse	= attenuation * intensity * diff * light.diffuse * materialPoint;
	vec3 specular	= attenuation * intensity * spec * light.specular * specularPoint;

	float shadow = calculateShadow(fragPosLightSpace, light);

//	return ambient + diffuse + specular;
	return ambient + (1.0f - shadow) * (diffuse + specular);
//	return vec3(1.0f - shadow);
}

void main() {
	vec3 viewDirection = normalize(viewPosition - vs_out.fragmentPosition);

	vec4 diffuse = texture(material.albedo, vs_out.texCoord);
	vec3 materialPoint = diffuse.rgb * material.tint;
	vec3 specularPoint = texture(material.metallic, vs_out.texCoord).rgb;

	vec3 result = vec3(0.f);

	vec3 normal = normalize(vs_out.normal);

	for (int i = 0; i < lightCount; ++i) {
		if (lights[i].type == 0) {
			result += calcPointLight(lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else if (lights[i].type == 1) {
			result += calcDirectionalLight(vs_out.fragPosLightSpace.positions[i], lights[i], normal, viewDirection, materialPoint, specularPoint);
		} else {
			result += calcSpotLight(vs_out.fragPosLightSpace.positions[i], lights[i], normal, viewDirection, materialPoint, specularPoint);
		}
	}

//	color = vec4(vec3(1.0f - linearDepth(gl_FragCoord.z) / far), 1.0f);
	color = vec4(result, diffuse.a);
}