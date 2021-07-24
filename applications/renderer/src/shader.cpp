
#include "shader.h"

#include <QFile>
#include <QTextStream>
#include <iostream>

Shader::Shader(const QString &vs_name, const QString &fs_name) : m_vs(0), m_fs(0), m_program(0)
{
    m_vs = CompileShader(vs_name, GL_VERTEX_SHADER);
    m_fs = CompileShader(fs_name, GL_FRAGMENT_SHADER);
    m_program = LinkProgram({m_vs, m_fs});
}

Shader::~Shader()
{
    if (m_program)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void Shader::Bind()
{
    if (m_program)
    {
        glUseProgram(m_program);
    }
}

void Shader::Release()
{
    glUseProgram(0);
}

GLuint Shader::CompileShader(const QString &filename, GLenum type)
{
    std::cout << "Compiling glsl shader: " << filename.toStdString() << std::endl;

    const std::string glsl_str = ReadShaderFile(filename);
    if (glsl_str.empty())
    {
        std::cout << "Cannot read shader source file: " << filename.toStdString() << std::endl;
        return 0;
    }
    const GLchar *src_buffer = glsl_str.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src_buffer, nullptr);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader");

    return shader;
}

GLuint Shader::LinkProgram(std::initializer_list<GLuint> shaders)
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

std::string Shader::ReadShaderFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "Could not open file: " << filename.toStdString() << std::endl;
        return "";
    }

    QTextStream file_stream(&file);
    QString file_str = file_stream.readAll();
    file.close();

    return file_str.toStdString();
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool is_program, const std::string &error_message)
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
