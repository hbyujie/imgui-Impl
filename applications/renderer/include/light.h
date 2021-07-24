#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "shader.h"

struct DirectLight
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 direction;
	bool enabled{ false };
};

struct PointLight
{
    bool enabled{false};
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };
};
const int MAX_POINT_LIGHTS = 7;

struct SpotLight
{
    bool enabled{false};
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
	float constant{ 1.0f };
	float linear{ 0.09f };
	float quadratic{ 0.032f };
    float cut_off = glm::cos(glm::radians(12.5f));
    float outer_cut_off = glm::cos(glm::radians(15.0f));
};
const int MAX_SPOT_LIGHTS = 7;

class Light
{
public:
	
	static void LinkUniform(const std::shared_ptr<Shader>& shader, const DirectLight& light)
	{
		shader->LinkUniformVec3("directLight.ambient", light.ambient);
		shader->LinkUniformVec3("directLight.diffuse", light.diffuse);
		shader->LinkUniformVec3("directLight.specular", light.specular);
		shader->LinkUniformVec3("directLight.direction", light.direction);
		shader->LinkUniformBool("directLight.enabled", light.enabled);
	}
};