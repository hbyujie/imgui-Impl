#version 460 core

in VS_OUT
{
	vec3 FragPos;
    vec3 FragNor;
	vec2 FragTex;

	mat3 TBN;
} fs_in;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D albedoMap;
layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 2) uniform sampler2D metallicMap;
layout(binding = 3) uniform sampler2D roughnessMap;
layout(binding = 4) uniform sampler2D aoMap;

uniform bool use_albedo_texture;
uniform bool use_normal_texture;
uniform bool use_metallic_texture;
uniform bool use_roughness_texture;
uniform bool use_ao_texture;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

struct DirectLight
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    vec3 direction;
	bool enabled;
};
uniform DirectLight directLight;

uniform vec3 eyePosition;


vec3 GetDirectLightColor(vec3 normal, vec3 eyeDirection, DirectLight light);

//vec3 GetLightColor(vec3 normal, vec3 eyeDirection, DirectLight light);

void main()
{	
	vec3 normal = normalize(fs_in.FragNor);
	vec3 eyeDirection = normalize(eyePosition - fs_in.FragPos);
	if(use_normal_texture)
	{
		normal = normalize(vec3(texture(normalMap, fs_in.FragTex)) * 2.0f - 1.0f);
		eyeDirection  = normalize(fs_in.TBN * eyeDirection);
	}

	vec3 lightColor = vec3(0.0);

	lightColor += GetDirectLightColor(normal, eyeDirection, directLight);


	FragColor = vec4(min(lightColor, vec3(1.0)), 1.0);
}

vec3 GetDirectLightColor(vec3 normal, vec3 eyeDirection, DirectLight light)
{
	if(!light.enabled)
	{
		return vec3(0.0);
	}
	
	vec3 lightDirection = normalize(light.direction);
	if(use_normal_texture)
	{
		lightDirection  = normalize(fs_in.TBN * lightDirection);
	}

	vec3 halfVector = normalize(lightDirection + eyeDirection);

	float diff = max(0.0, dot(normal, lightDirection));
	float spec = 0.0;
	if(diff != 0.0)
	{
		spec = pow(max(0.0, dot(normal, halfVector)), material.shininess);
	}

	vec3 materialAmbient = material.ambient;
	vec3 materialDiffuse = material.diffuse;
	vec3 materialSpecular = material.specular;

	if(use_albedo_texture)
	{
		materialAmbient = vec3(texture(albedoMap, fs_in.FragTex));
		materialDiffuse = materialAmbient;
	}

	if(use_metallic_texture)
	{
		materialSpecular = vec3(texture(metallicMap, fs_in.FragTex));
	}

	vec3 ambient = light.ambient * materialAmbient;
    vec3 diffuse = light.diffuse * diff * materialDiffuse;
    vec3 specular = light.specular * spec * materialSpecular;

	return (ambient + diffuse + specular);
}