
#include "gl_shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

GLShader::GLShader(const std::string &vs_name, const std::string &fs_name) : m_vs(0), m_fs(0), m_program(0)
{
    m_vs = CompileShader(vs_name, GL_VERTEX_SHADER);
    m_fs = CompileShader(fs_name, GL_FRAGMENT_SHADER);
    m_program = LinkProgram({m_vs, m_fs});
}

GLShader::~GLShader()
{
    if (m_program)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void GLShader::Bind()
{
    if (m_program)
    {
        glUseProgram(m_program);
    }
}

void GLShader::Release()
{
    glUseProgram(0);
}

GLuint GLShader::CompileShader(const std::string &filename, GLenum type)
{
    std::cout << "Compiling glsl shader: " << filename << std::endl;

    const std::string glsl_str = ReadShaderFile(filename);
    if (glsl_str.empty())
    {
        std::cout << "Cannot read shader source file: " << filename << std::endl;
        return 0;
    }
    const GLchar *src_buffer = glsl_str.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src_buffer, nullptr);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader");

    return shader;
}

GLuint GLShader::LinkProgram(std::initializer_list<GLuint> shaders)
{
    std::cout << "Linking glsl program : ";

    GLuint program = glCreateProgram();

    for (GLuint shader : shaders)
    {
        glAttachShader(program, shader);
        std::cout << "shader: " << shader << ", ";
    }
    std::cout << "program: " << program << std::endl;

    glLinkProgram(program);

    for (GLuint shader : shaders)
    {
        glDetachShader(program, shader);
        glDeleteShader(shader);
        shader = 0;
    }

    CheckShaderError(program, GL_LINK_STATUS, true, "Error linking program");

    return program;
}

std::string GLShader::ReadShaderFile(const std::string &filename)
{
    std::ifstream file{filename};
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void GLShader::CheckShaderError(GLuint shader, GLuint flag, bool is_program, const std::string &error_message)
{
    GLint success = 0;
    GLchar error[1024] = {0};

    if (is_program)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if (success == GL_FALSE)
    {
        if (is_program)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << error_message << ": '" << error << "'" << std::endl;
    }
}