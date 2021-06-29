
#pragma once
#include "opengl/gl_defines.h"

#include <string>
#include <memory>

class GLMesh;

class ModelReader
{
  public:
    ModelReader(const std::string &filename, const std::shared_ptr<GLMesh> mesh);
};
