#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

class TextureMap
{
  public:
    explicit TextureMap(const std::string &file_name, const bool is_flip = true);
    ~TextureMap();

    const GLuint GetTextureID() const
    {
        return m_id;
    }

  protected:
    void LoadTexture(char const *path);

    void DeleteTexture();

  private:
    std::string m_file_name{""};
    int m_width{0};
    int m_height{0};
	int m_components{ 0 };
	int m_levels{ 0 };
	GLuint m_id{0};
};
