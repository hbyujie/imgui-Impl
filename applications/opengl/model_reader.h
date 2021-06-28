
#pragma once

#include "opengl/gl_defines.h"
#include <string>

class ModelReader
{
  public:
    ModelReader(const std::string &filename, Mesh *mesh);
};
