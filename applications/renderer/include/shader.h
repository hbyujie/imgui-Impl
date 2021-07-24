
#pragma once

#include <GL/glew.h>
#include <QString>
#include <glm/glm.hpp>
#include <string>

class Shader
{
  public:
    Shader(const QString &vs_name, const QString &fs_name);
    ~Shader();

    const GLuint GetProgram() const
    {
        return m_program;
    }

    void Bind();

    void Release();

    // utility uniform functions
    // ------------------------------------------------------------------------
    void LinkUniformBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void LinkUniformInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void LinkUniformFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void LinkUniformVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
    }
    void LinkUniformVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_program, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void LinkUniformVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
    }
    void LinkUniformVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_program, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void LinkUniformVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(m_program, name.c_str()), 1, &value[0]);
    }
    void LinkUniformVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(m_program, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void LinkUniformMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void LinkUniformMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void LinkUniformMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void LinkUniformBlock(const std::string &block_name, const GLuint block_binding)
    {
        GLuint block_index = glGetUniformBlockIndex(m_program, block_name.c_str());
        glUniformBlockBinding(m_program, block_index, block_binding);
    }

  private:
    GLuint CompileShader(const QString &filename, GLenum type);
    GLuint LinkProgram(std::initializer_list<GLuint> shaders);

    std::string ReadShaderFile(const QString &filename);
    void CheckShaderError(GLuint shader, GLuint flag, bool is_program, const std::string &error_message);

  private:
    GLuint m_vs, m_fs;
    GLuint m_program;
};
