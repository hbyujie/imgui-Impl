#version 460 core

#include_part

struct AmbientLight
{
	vec3 color;
	bool enabled;
};

vec3 GetAmbientLightColor(AmbientLight ambientLight);

#definition_part

vec3 GetAmbientLightColor(AmbientLight ambientLight)
{
    return ambientLight.enabled ? ambientLight.color : vec3(0.0, 0.0, 0.0);
}