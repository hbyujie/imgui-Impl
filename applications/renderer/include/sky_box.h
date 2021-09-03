#pragma once

#include <GL/glew.h>
#include <QString>
#include <memory>
#include <vector>

class Shader;

class SkyBox
{
  public:
    SkyBox();
    ~SkyBox();

    void SetTextures(const std::vector<QString> &cube_texture);

    void SetTextures(const std::vector<std::string> &faces);

    void Draw(const std::shared_ptr<Shader> &shader);

  private:
    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    // -------------------------------------------------------
    GLuint loadCubemap(const std::vector<std::string> &faces);

  private:
    // primitive buffer
    GLuint m_vao{0};
    GLuint m_vbo{0};

    GLuint m_cube_texture{0};
};