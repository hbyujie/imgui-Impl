#version 450 core

layout(binding = 0) uniform sampler2D albedoMap;
layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 2) uniform sampler2D metallicMap;
layout(binding = 3) uniform sampler2D roughnessMap;
layout(binding = 4) uniform sampler2D aoMap;

struct DirectLight
{
	bool enabled;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirectLight dirLight;

struct PointLight
{
	bool enabled;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};
#define MAX_POINT_LIGHTS 7
uniform PointLight pointLights[MAX_POINT_LIGHTS];

struct SpotLight
{
	bool enabled;
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};
#define MAX_SPOT_LIGHTS 7
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform vec3 eyePosition;

layout(location = 0) in Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
} vin;

out vec4 FragColor;

// function prototypes
vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 eyeDirection);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 eyeDirection);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 eyeDirection);

void main()
{
	vec3 EyeDirection = normalize(eyePosition - vin.position);

	vec3 LightColor = vec3(0.0);
	LightColor += CalcDirectLight(dirLight, vin.normal, EyeDirection);

	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
		LightColor += CalcPointLight(pointLights[i], vin.normal, vin.position, EyeDirection);

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
		LightColor += CalcSpotLight(spotLights[i], vin.normal, vin.position, EyeDirection);

	FragColor = vec4(min(LightColor, vec3(1.0)), 1.0);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 eyeDirection)
{
	if(!light.enabled)
		return vec3(0.0);

	vec3 lightDirection = normalize(light.direction);
	vec3 halfVector = normalize(lightDirection + eyeDirection);
	
	float diff = max(0.0, dot(normal, lightDirection));
	float spec = max(0.0, dot(normal, halfVector));
	
	if(diff == 0.0)
		spec = 0.0;
	else
		spec = pow(spec, 32.0f);
	
	vec3 ambient = light.ambient * vec3(texture(albedoMap, vin.texcoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(albedoMap, vin.texcoord));
    vec3 specular = light.specular * spec * vec3(texture(metallicMap, vin.texcoord));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 eyeDirection)
{
	if(!light.enabled)
		return vec3(0.0);

	vec3 lightDirection = normalize(light.position - fragPos);
	vec3 halfVector = normalize(lightDirection + eyeDirection);

	float diff = max(0.0, dot(normal, lightDirection));
	float spec = max(0.0, dot(normal, halfVector));
	
	if(diff == 0.0)
		spec = 0.0;
	else
		spec = pow(spec, 32.0f);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

	vec3 ambient = light.ambient * vec3(texture(albedoMap, vin.texcoord)) * attenuation;
    vec3 diffuse = light.diffuse * diff * vec3(texture(albedoMap, vin.texcoord)) * attenuation;
    vec3 specular = light.specular * spec * vec3(texture(metallicMap, vin.texcoord)) * attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 eyeDirection)
{
	if(!light.enabled)
		return vec3(0.0);

	vec3 lightDirection = normalize(light.position - fragPos);
	vec3 halfVector = normalize(lightDirection + eyeDirection);

	float diff = max(0.0, dot(normal, lightDirection));
	float spec = max(0.0, dot(normal, halfVector));
	
	if(diff == 0.0)
		spec = 0.0;
	else
		spec = pow(spec, 32.0f);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

    float theta = dot(lightDirection, normalize(light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(albedoMap, vin.texcoord)) * attenuation;
    vec3 diffuse = light.diffuse * diff * vec3(texture(albedoMap, vin.texcoord)) * attenuation * intensity;
    vec3 specular = light.specular * spec * vec3(texture(metallicMap, vin.texcoord)) * attenuation * intensity;

    return (ambient + diffuse + specular);
}