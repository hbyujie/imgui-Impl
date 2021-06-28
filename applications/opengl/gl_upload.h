#pragma once

#include "opengl/gl_defines.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class GLUpload
{
  public:
    static void UploadVertex(PartsMesh &parts_mesh);
};