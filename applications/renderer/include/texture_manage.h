#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

struct Texture
{
    unsigned int id{0};
    std::string file_name{""};
    int width, height;
    int levels{0};
    bool is_vertical_flip{true};
};

class TextureManage
{
  public:
    static GLuint LoadTexture(char const *path);

	static void DeleteTexture(GLuint &textureID);

  protected:
    void CreateTexture(Texture &texture);

    void CreateCubeTexture(const std::vector<std::string> &cube_texture, GLuint &texture);

    void DeleteTexture(Texture &texture);
	};