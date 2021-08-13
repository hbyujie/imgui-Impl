#pragma once

#include <GL/glew.h>
#include <memory>

class Shader;

class Quad
{
  public:
    Quad();
    ~Quad();

    void Draw(const std::shared_ptr<Shader> shader, const GLuint &texture);

  private:
    GLuint m_vao{0};
    GLuint m_vbo{0};
};