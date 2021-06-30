#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "opengl/gl_defines.h"

class GLMath
{
  public:
    static void ComputeBoundingSphereAverage(const std::vector<glm::vec3> &positions, glm::vec3 *center, float *radius);

    static void ComputeBoundingSphereRitter(const std::vector<glm::vec3> &positions, glm::vec3 *center, float *radius);

	static void ComputeTangents(std::vector<Vertex>& vertices);

};