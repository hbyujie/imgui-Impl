#pragma once

#include <glm/glm.hpp>
#include <memory>

class GLShader;

struct DirectLight
{
    bool enabled{false};
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
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

class GLLight
{
  public:
    GLLight();
    ~GLLight();

    void BindToShader(std::shared_ptr<GLShader> shader);

    void InitDirectLight();
    DirectLight &GetVariableDirectLight()
    {
        return m_direct_light;
    }
    const DirectLight &GetConstDirectLight() const
    {
        return m_direct_light;
    }

    void InitPointLights();
    PointLight *GetVariablePointLights()
    {
        return m_point_lights;
    }

    void InitSpotLights();
    SpotLight *GetVariableSpotLights()
    {
        return m_spot_lights;
    }

  private:
    DirectLight m_direct_light;
    PointLight m_point_lights[MAX_POINT_LIGHTS];
    SpotLight m_spot_lights[MAX_SPOT_LIGHTS];
};
