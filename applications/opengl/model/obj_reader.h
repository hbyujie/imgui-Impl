#pragma once

#include "opengl/gl_defines.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

class GLMesh;

struct ObjFace
{
    std::string usemtl;
    std::vector<uint32_t> pos_indices;
    std::vector<uint32_t> nor_indices;
    std::vector<uint32_t> tex_indices;
    Textures textures;
    Material material;
};

struct ObjMesh
{
    std::string mtllib;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texcoords;
    std::vector<ObjFace> faces;
};

struct MeshMark
{
    bool isNewMesh = false;
    std::string g = "";
    std::string s = "";
    std::string usemtl = "";

    void Clear()
    {
        isNewMesh = false;
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

    void GetMesh(const std::shared_ptr<GLMesh> mesh);

  private:
    void ParseObj(const std::string &filename, std::string *mtllib);

    void ParseMtl(const std::string &filename, const std::string &dir);

    void ParseLinebuf(const char *linebuf, std::vector<uint32_t> &vIndex, std::vector<uint32_t> &vtIndex,
                      std::vector<uint32_t> &vnIndex);

  private:
    std::string m_mtllib{""};
    std::string m_directory{""};
    ObjMesh m_mesh;
};
