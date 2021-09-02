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


void main()
{	
	FragColor = vec4(vec3(texture(albedoMap, fs_in.FragTex)), 1.0);
}
