
#include "gl_texture_manager.h"
#include "image/Image.hpp"

GLTextureManager::GLTextureManager()
{
    // int channels = 3;
    // bool isVerticalFlip = textures.is_vertical_flip;
    // GLenum format = GL_RGB;
    // GLenum internalformat = GL_RGB8;
    // int levels = 0;
}

GLTextureManager::~GLTextureManager()
{
}

void GLTextureManager::UploadTextures(Textures &textures)
{
    std::string file_name;

    file_name = textures.albedo.file_name;
    if (!file_name.empty())
    {
        if (m_textures.find(file_name) == m_textures.end())
        {
            textures.albedo =
                createTexture(Image::fromFile(file_name, 3, textures.albedo.is_vertical_flip), GL_RGB, GL_RGB8, 0);
            textures.albedo.file_name = file_name;
            m_textures[file_name] = textures.albedo;
        }
        else
        {
            textures.albedo = m_textures[file_name];
        }
    }

    file_name = textures.normal.file_name;
    if (!file_name.empty())
    {
        if (m_textures.find(file_name) == m_textures.end())
        {
            textures.normal =
                createTexture(Image::fromFile(file_name, 3, textures.normal.is_vertical_flip), GL_RGB, GL_RGB8, 0);
            textures.normal.file_name = file_name;
            m_textures[file_name] = textures.normal;
        }
        else
        {
            textures.normal = m_textures[file_name];
        }
    }

    file_name = textures.metallic.file_name;
    if (!file_name.empty())
    {
        if (m_textures.find(file_name) == m_textures.end())
        {
            textures.metallic =
                createTexture(Image::fromFile(file_name, 3, textures.metallic.is_vertical_flip), GL_RED, GL_R8, 0);
            textures.metallic.file_name = file_name;
            m_textures[file_name] = textures.metallic;
        }
        else
        {
            textures.metallic = m_textures[file_name];
        }
    }

    file_name = textures.roughness.file_name;
    if (!file_name.empty())
    {
        if (m_textures.find(file_name) == m_textures.end())
        {
            textures.roughness =
                createTexture(Image::fromFile(file_name, 3, textures.roughness.is_vertical_flip), GL_RED, GL_R8, 0);
            textures.roughness.file_name = file_name;
            m_textures[file_name] = textures.roughness;
        }
        else
        {
            textures.roughness = m_textures[file_name];
        }
    }

    file_name = textures.ao.file_name;
    if (!file_name.empty())
    {
        if (m_textures.find(file_name) == m_textures.end())
        {
            textures.ao = createTexture(Image::fromFile(file_name, 3, textures.ao.is_vertical_flip), GL_RED, GL_R8, 0);
            textures.ao.file_name = file_name;
            m_textures[file_name] = textures.ao;
        }
        else
        {
            textures.ao = m_textures[file_name];
        }
    }
}

Texture GLTextureManager::createTexture(GLenum target, int width, int height, GLenum internalformat, int levels) const
{
    Texture texture;
    texture.width = width;
    texture.height = height;
    texture.levels = (levels > 0) ? levels : numMipmapLevels(width, height);

    glCreateTextures(target, 1, &texture.id);
    glTextureStorage2D(texture.id, texture.levels, internalformat, width, height);
    glTextureParameteri(texture.id, GL_TEXTURE_MIN_FILTER, texture.levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTextureParameteri(texture.id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameterf(texture.id, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_max_anisotropy);
    return texture;
}

Texture GLTextureManager::createTexture(const std::shared_ptr<class Image> &image, GLenum format, GLenum internalformat,
                                        int levels) const
{
    if (image == nullptr)
    {
        return Texture();
    }

    Texture texture = createTexture(GL_TEXTURE_2D, image->width(), image->height(), internalformat, levels);
    if (image->isHDR())
    {
        glTextureSubImage2D(texture.id, 0, 0, 0, texture.width, texture.height, format, GL_FLOAT,
                            image->pixels<float>());
    }
    else
    {
        glTextureSubImage2D(texture.id, 0, 0, 0, texture.width, texture.height, format, GL_UNSIGNED_BYTE,
                            image->pixels<unsigned char>());
    }

    if (texture.levels > 1)
    {
        glGenerateTextureMipmap(texture.id);
    }
    return texture;
}

int GLTextureManager::numMipmapLevels(int width, int height) const
{
    int levels = 1;
    while ((width | height) >> levels)
    {
        ++levels;
    }
    return levels;
}

void GLTextureManager::deleteTexture(Texture &texture)
{
    glDeleteTextures(1, &texture.id);
    std::memset(&texture, 0, sizeof(Texture));
}