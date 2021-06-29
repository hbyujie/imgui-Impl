#include "opengl/gl_light.h"
#include "opengl/gl_shader.h"

GLLight::GLLight()
{
}

GLLight::~GLLight()
{
}

void GLLight::BindToShader(std::shared_ptr<GLShader> shader)
{
    if (shader)
    {
        shader->LinkUniformBool("dirLight.enabled", m_direct_light.enabled);
        shader->LinkUniformVec3("dirLight.direction", m_direct_light.direction);
        shader->LinkUniformVec3("dirLight.ambient", m_direct_light.ambient);
        shader->LinkUniformVec3("dirLight.diffuse", m_direct_light.diffuse);
        shader->LinkUniformVec3("dirLight.specular", m_direct_light.specular);

        for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
        {
            const std::string pre_name = "pointLights[" + std::to_string(i) + "]";
            shader->LinkUniformBool(pre_name + ".enabled", m_point_lights[i].enabled);
            shader->LinkUniformVec3(pre_name + ".position", m_point_lights[i].position);
            shader->LinkUniformVec3(pre_name + ".ambient", m_point_lights[i].ambient);
            shader->LinkUniformVec3(pre_name + ".diffuse", m_point_lights[i].diffuse);
            shader->LinkUniformVec3(pre_name + ".specular", m_point_lights[i].specular);
            shader->LinkUniformFloat(pre_name + ".constant", m_point_lights[i].constant);
            shader->LinkUniformFloat(pre_name + ".linear", m_point_lights[i].linear);
            shader->LinkUniformFloat(pre_name + ".quadratic", m_point_lights[i].quadratic);
        }

		for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
		{
			const std::string pre_name = "spotLights[" + std::to_string(i) + "]";
			shader->LinkUniformBool(pre_name + ".enabled", m_spot_lights[i].enabled);
			shader->LinkUniformVec3(pre_name + ".position", m_spot_lights[i].position);
			shader->LinkUniformVec3(pre_name + ".direction", m_spot_lights[i].direction);
			shader->LinkUniformVec3(pre_name + ".ambient", m_spot_lights[i].ambient);
			shader->LinkUniformVec3(pre_name + ".diffuse", m_spot_lights[i].diffuse);
			shader->LinkUniformVec3(pre_name + ".specular", m_spot_lights[i].specular);
			shader->LinkUniformFloat(pre_name + ".constant", m_spot_lights[i].constant);
			shader->LinkUniformFloat(pre_name + ".linear", m_spot_lights[i].linear);
			shader->LinkUniformFloat(pre_name + ".quadratic", m_spot_lights[i].quadratic);
			shader->LinkUniformFloat(pre_name + ".cutOff", m_spot_lights[i].cut_off);
			shader->LinkUniformFloat(pre_name + ".outerCutOff", m_spot_lights[i].outer_cut_off);
		}
    }
}

void GLLight::InitDirectLight()
{
    m_direct_light.enabled = true;
    m_direct_light.direction = glm::vec3(0.2f, 1.0f, 0.3f);
    m_direct_light.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    m_direct_light.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    m_direct_light.specular = glm::vec3(0.5f, 0.5f, 0.5f);
}

void GLLight::InitPointLights()
{
    // point light 1
    m_point_lights[0].enabled = true;
    m_point_lights[0].position = glm::vec3(0.7f, 2.2f, 2.0f);
    m_point_lights[0].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    m_point_lights[0].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    m_point_lights[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    m_point_lights[0].constant = 1.0f;
    m_point_lights[0].linear = 0.09f;
    m_point_lights[0].quadratic = 0.032f;

    // point light 2
    m_point_lights[1].enabled = true;
    m_point_lights[1].position = glm::vec3(2.3f, -1.3f, -4.0f);
    m_point_lights[1].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    m_point_lights[1].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    m_point_lights[1].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    m_point_lights[1].constant = 1.0f;
    m_point_lights[1].linear = 0.09f;
    m_point_lights[1].quadratic = 0.032f;

    // point light 3
    m_point_lights[2].enabled = true;
    m_point_lights[2].position = glm::vec3(-4.0f, 4.0f, -12.0f);
    m_point_lights[2].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    m_point_lights[2].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    m_point_lights[2].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    m_point_lights[2].constant = 1.0f;
    m_point_lights[2].linear = 0.09f;
    m_point_lights[2].quadratic = 0.032f;

    // point light 4
    m_point_lights[3].enabled = true;
    m_point_lights[3].position = glm::vec3(0.0f, 2.0f, -3.0f);
    m_point_lights[3].ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    m_point_lights[3].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    m_point_lights[3].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    m_point_lights[3].constant = 1.0f;
    m_point_lights[3].linear = 0.09f;
    m_point_lights[3].quadratic = 0.032f;
}

void GLLight::InitSpotLights()
{
    m_spot_lights[0].enabled = true;
    m_spot_lights[0].position = glm::vec3(0.0f, 2.0f, 3.0f);
    m_spot_lights[0].direction = glm::vec3(0.0f, 0.0f, 1.0f);
    m_spot_lights[0].ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    m_spot_lights[0].diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    m_spot_lights[0].specular = glm::vec3(1.0f, 1.0f, 1.0f);
    m_spot_lights[0].constant = 1.0f;
    m_spot_lights[0].linear = 0.09f;
    m_spot_lights[0].quadratic = 0.032f;
    m_spot_lights[0].cut_off = glm::cos(glm::radians(12.5f));
    m_spot_lights[0].outer_cut_off = glm::cos(glm::radians(15.0f));
}