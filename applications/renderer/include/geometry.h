#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include "texture_manage.h"

struct PrimitiveBuffer
{
    GLenum mode{0};

    GLuint vao{0};

    GLuint vbo_position{0};
    GLuint vbo_normal{0};
    GLuint vbo_texcoord{0};
    GLuint vbo_color{0};
    GLuint vbo_tangent{0};
    GLuint vbo_bitangent{0};

    GLuint ebo_position{0};
    GLuint ebo_normal{0};
    GLuint ebo_texcoord{0};
    GLuint ebo_color{0};
    GLuint ebo_tangent{0};
    GLuint ebo_bitangent{0};

    GLint first{0};
    GLsizei count{0};
};

struct TextureBuffer
{
    Texture albedo;
    Texture normal;
    Texture metallic;
    Texture roughness;
    Texture ao;
};

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess{1.0f};
};

class Shader;

class Geometry : public TextureManage
{
  public:
    Geometry();
    ~Geometry();

    void SetName(const std::string &name);

    void SetMode(const std::string &name, GLenum mode);
    void SetVertexArray(const std::string &name, std::vector<glm::vec3> &array);
    void SetNormalArray(const std::string &name, std::vector<glm::vec3> &array);
    void SetTexcoordArray(const std::string &name, std::vector<glm::vec2> &array);
    void SetColorArray(const std::string &name, std::vector<glm::vec3> &array);
    void SetTangentArray(const std::string &name, std::vector<glm::vec3> &array);
    void SetBitangentArray(const std::string &name, std::vector<glm::vec3> &array);
    void SetIndiceArray(const std::string &name, std::vector<uint32_t> &array);
    void CreatePrimitiveBuffers();
    void DeletePrimitiveBuffers();

    void SetTextures(const std::string &name, const TextureBuffer &texture_buffer);
    void CreatePrimitiveTextures();
    void DeletePrimitiveTextures();

    void SetMaterial(const std::string &name, const Material &material);
	
    void Draw(const std::shared_ptr<Shader> &shader);

    void UpdateBoundingBox();
    const glm::vec3 &GetBoundingBoxCenter() const
    {
        return m_center;
    }
    const float &GetBoundingBoxRadius() const
    {
        return m_radius;
    }

  private:
    void UpdateBoundingBox(const std::vector<glm::vec3> &positions);

    void DeletePrimitiveBuffer(PrimitiveBuffer &textures);
	
    void DeleteTextureBuffer(TextureBuffer &textures);

  private:
    std::string m_name;

    glm::mat4 m_model{glm::mat4(1.0f)};

    // data
    std::unordered_map<std::string, GLenum> m_modes;
    std::unordered_map<std::string, std::vector<glm::vec3>> m_positions;
    std::unordered_map<std::string, std::vector<glm::vec3>> m_normals;
    std::unordered_map<std::string, std::vector<glm::vec2>> m_texcoords;
    std::unordered_map<std::string, std::vector<glm::vec3>> m_colors;
    std::unordered_map<std::string, std::vector<glm::vec3>> m_tangents;
    std::unordered_map<std::string, std::vector<glm::vec3>> m_bitangents;
    std::unordered_map<std::string, std::vector<uint32_t>> m_indices;

    // primitive buffer
    std::unordered_map<std::string, PrimitiveBuffer> m_primitive_buffer_map;

    //
    std::unordered_map<std::string, TextureBuffer> m_texture_buffer_map;

    //
    std::unordered_map<std::string, Material> m_material_map;
	
    glm::vec3 m_center;
    float m_radius;
};