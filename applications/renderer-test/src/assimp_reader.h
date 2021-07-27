#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Assimp
{
struct Usemtl
{
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float ns{0.f};
    float ni{1.f};
    float d{1.f};
    float illum{1.f};
    std::string map_kd;
    std::string map_bump;
    std::string map_ks;

    std::vector<uint32_t> indices;
};

struct Model
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    Usemtl usemtl;
};

class AssimpReader
{
  public:
    AssimpReader() = delete;
    explicit AssimpReader(const std::string &filename, const std::string &dir);

    // use swap to replace copy data array
    std::unordered_map<std::string, Model> &GetModeles()
    {
        return m_models;
    }

    const std::string &GetModelName()
    {
        return m_model_name;
    }

  private:
    void CruiseNode(const aiNode *node, const aiScene *scene, const std::string directory);

    void ParseMesh(const aiMesh *aMesh, Model *model);

    void ParseMaterial(const aiMaterial *aMaterial, const std::string directory, Usemtl *usemtle);

    std::unordered_map<std::string, Model> m_models;
    std::string m_model_name;
};
} // namespace Assimp