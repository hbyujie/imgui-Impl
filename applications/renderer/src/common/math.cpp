#include "common/math.h"

namespace OpenGL
{
void Math::ComputeTangents(const std::vector<glm::vec3> &positions, const std::vector<glm::vec2> &texcoords,
                           std::vector<glm::vec3> *tangents, std::vector<glm::vec3> *bitangents)
{
    const unsigned int vertex_size = positions.size();
    assert(vertex_size % 3 == 0);

    for (unsigned int i = 0; i < vertex_size; i += 3)
    {
        unsigned int index1 = i;
        unsigned int index2 = i + 1;
        unsigned int index3 = i + 2;

        glm::vec3 pos1 = positions[index1];
        glm::vec3 pos2 = positions[index2];
        glm::vec3 pos3 = positions[index3];

        glm::vec2 uv1 = texcoords[index1];
        glm::vec2 uv2 = texcoords[index2];
        glm::vec2 uv3 = texcoords[index3];

        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent;
        tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent = glm::normalize(tangent);
        tangents->push_back(tangent);
        tangents->push_back(tangent);
        tangents->push_back(tangent);

        glm::vec3 bitangent;
        bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        bitangent = glm::normalize(bitangent);
		bitangents->push_back(bitangent);
		bitangents->push_back(bitangent);
		bitangents->push_back(bitangent);
	}
}
} // namespace OpenGL