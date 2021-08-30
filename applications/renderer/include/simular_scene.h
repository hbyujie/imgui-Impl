
#pragma once

#include <GL/glew.h>
#include <memory>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader;

struct Primitive
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    std::vector<uint32_t> indices;

    std::string albedo_file;
    std::string normal_file;
    std::string metallic_file;
    std::string roughness_file;
    std::string ao_file;

    float line_width{0.0};
    float point_size{0.0};

    GLenum mode{0};
};

struct OpenGL_Primitive
{
    GLuint vao{0};

    GLuint vbo_position{0};
    GLuint vbo_normal{0};
    GLuint vbo_texcoord{0};
    GLuint vbo_color{0};
    GLuint vbo_tangent{0};
    GLuint vbo_bitangent{0};

    GLuint ebo{0};
    GLint first{0};
    GLsizei count{0};

    GLuint albedo{0};
    GLuint normal{0};
    GLuint metallic{0};
    GLuint roughness{0};
    GLuint ao{0};

    float line_width{0.0};
    float point_size{0.0};

    GLenum mode{0};

    glm::mat4 model_matrix{glm::mat4(1.0f)};
};

class SimularScene
{
  public:
    SimularScene();
    ~SimularScene();

    void AddPrimitive(const std::string &name, const Primitive &primitive);

    void Draw(const std::shared_ptr<Shader> shader);

  private:
    void CreateOpenGLPrimitive(const Primitive &primitive, OpenGL_Primitive *gl_primitive);
    void DeleteOpenGLPrimitive(OpenGL_Primitive *gl_primitive);

    GLuint LoadTexture(const std::string &path);

    std::unordered_map<std::string, Primitive> m_primitives;
    std::unordered_map<std::string, OpenGL_Primitive> m_gl_primitives;
};
