#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace OpenGL
{
class Math
{
  public:
    static void ComputeTangents(const std::vector<glm::vec3> &positions, const std::vector<glm::vec2> &texcoords,
                                std::vector<glm::vec3> *tangents, std::vector<glm::vec3> *bitangents);
};
} // namespace OpenGL