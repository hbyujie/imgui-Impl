
#pragma once

#include "gl_defines.h"
#include <GL/glew.h>
#include <memory>
#include <string>
#include <unordered_map>

class GLTextureManager
{
  public:
    GLTextureManager();
    ~GLTextureManager();

    void UploadTextures(Textures &textures);

  private:
    Texture createTexture(const std::shared_ptr<class Image> &image, GLenum format, GLenum internalformat,
                          int levels = 0) const;
    Texture createTexture(GLenum target, int width, int height, GLenum internalformat, int levels = 0) const;
    int numMipmapLevels(int width, int height) const;

    void deleteTexture(Texture &texture);

  private:
    float m_max_anisotropy;
	Textures mtextures;

  private:
    std::unordered_map<std::string, Texture> m_textures;
};
