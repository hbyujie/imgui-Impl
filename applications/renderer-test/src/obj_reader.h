#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

namespace OBJ
{

struct Usemtl
{
    std::string usemtl;
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
};

struct Faces
{
    std::string usemtl;
    std::vector<uint32_t> pos_indices;
    std::vector<uint32_t> nor_indices;
    std::vector<uint32_t> tex_indices;
};

struct Model
{
    std::string mtllib;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<Faces> faces;
    std::vector<Usemtl> usemtles;
};

struct FaceMark
{
    bool is_new{false};
    std::string g{""};
    std::string s{""};
    std::string usemtl{""};

    void Clear()
    {
        is_new = false;
        g = "";
        s = "";
        usemtl = "";
    }
};

class ObjReader
{
  public:
    ObjReader() = delete;
    explicit ObjReader(const std::string &filename, const std::string &dir);

	bool IsSharedIndices();


    Model &GetModel()
    {
        return m_model;
    }

  private:
    void ParseObj(const std::string &filename, std::string *mtllib);

    void ParseMtl(const std::string &filename, const std::string &dir);

    void ParseLinebuf(const char *linebuf, std::vector<uint32_t> &vIndex, std::vector<uint32_t> &vtIndex,
                      std::vector<uint32_t> &vnIndex);

  private:
    Model m_model;
};
} // namespace OBJ
