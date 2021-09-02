#include "texture_map.h"
#include "stb/stb_image.h"

#include <iostream>

TextureMap::TextureMap(const std::string &file_name, const bool is_flip)
{
    m_file_name = file_name;

    stbi_set_flip_vertically_on_load(is_flip);
    LoadTexture(file_name.c_str());
}

TextureMap::~TextureMap()
{
    DeleteTexture();
}

void TextureMap::LoadTexture(char const *path)
{
    glGenTextures(1, &m_id);

    unsigned char *data = stbi_load(path, &m_width, &m_height, &m_components, 0);
    if (data)
    {
        GLenum format;
        if (m_components == 1)
            format = GL_RED;
        else if (m_components == 3)
            format = GL_RGB;
        else if (m_components == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}

void TextureMap::DeleteTexture()
{
    glDeleteTextures(1, &m_id);
    m_id = 0;
}
