#version 450 core

layout(binding = 0) uniform sampler2D albedoMap;
layout(binding = 1) uniform sampler2D normalMap;
layout(binding = 2) uniform sampler2D metallicMap;
layout(binding = 3) uniform sampler2D roughnessMap;
layout(binding = 4) uniform sampler2D aoMap;

uniform vec3 eyePosition;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec4 FragColor;

void main()
{
	vec3 Ambient = texture(albedoMap, texcoord).rgb;

	FragColor = vec4(Ambient, 1.0);
}