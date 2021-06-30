#include "obj_reader.h"
#include "opengl/gl_math.h"
#include "opengl/gl_mesh.h"

#include <fstream>
#include <iostream>
#include <sstream>

const uint32_t MaxLineLength = 256;

ObjReader::ObjReader(const std::string &filename, const std::string &dir)
{
    std::string mtllib{""};
    ParseObj(filename, &mtllib);
    ParseMtl(dir + mtllib, dir);
}

void ObjReader::GetMesh(const std::shared_ptr<GLMesh> mesh)
{
    glm::vec3 center;
    float radius;
    GLMath::ComputeBoundingSphereRitter(m_mesh.positions, &center, &radius);
    mesh->SetBoundingBox(center, radius);

    for (const auto &face : m_mesh.faces)
    {
        assert(face.pos_indices.size() == face.nor_indices.size());
        assert(face.pos_indices.size() == face.tex_indices.size());

        auto &parts_mesh = mesh->GetVariablePartsMeshes()[face.usemtl];

        const int vertex_size = face.pos_indices.size();
        parts_mesh.vertices.resize(vertex_size);
        for (int i = 0; i < vertex_size; ++i)
        {
            parts_mesh.vertices[i].position = m_mesh.positions[face.pos_indices[i]];
            parts_mesh.vertices[i].normal = m_mesh.normals[face.nor_indices[i]];
            parts_mesh.vertices[i].texcoord = m_mesh.texcoords[face.tex_indices[i]];
        }
        GLMath::ComputeTangents(parts_mesh.vertices);

        parts_mesh.textures = face.textures;
        parts_mesh.material = face.material;
    }
}

void ObjReader::ParseObj(const std::string &filename, std::string *mtllib)
{
    std::cout << "obj path: " << filename << std::endl;

    std::ifstream file(filename);
    if (!file)
    {
        return;
    }

    // some scratch memory
    char buffer[MaxLineLength];
    uint32_t index = 0;

    MeshMark meshMark;

    while (file)
    {
        file >> buffer;
        if (buffer[0] == 'o' || buffer[0] == '#')
        {
            // ignore smoothing groups, groups and objects
            char linebuf[MaxLineLength];
            file.getline(linebuf, MaxLineLength);
            // std::cout << "other Info: (" << buffer[0] << " " << linebuf << ")" << std::endl;
        }
        else if (strcmp(buffer, "mtllib") == 0)
        {
            file >> *mtllib;
        }
        else if (strcmp(buffer, "g") == 0)
        {
            meshMark.isNewMesh = true;
            file >> meshMark.g;
        }
        else if (strcmp(buffer, "s") == 0)
        {
            meshMark.isNewMesh = true;
            file >> meshMark.s;
        }
        else if (strcmp(buffer, "usemtl") == 0)
        {
            meshMark.isNewMesh = true;
            file >> meshMark.usemtl;
        }
        else if (strcmp(buffer, "v") == 0)
        {
            // positions
            float x, y, z;
            file >> x >> y >> z;
            glm::vec3 vertex(x, y, z);
            m_mesh.positions.push_back(vertex);
        }
        else if (strcmp(buffer, "vn") == 0)
        {
            // normals
            float x, y, z;
            file >> x >> y >> z;
            glm::vec3 normal(x, y, z);
            m_mesh.normals.push_back(normal);
        }
        else if (strcmp(buffer, "vt") == 0)
        {
            // texture coords
            float u, v;
            file >> u >> v;
            glm::vec2 texcoord(u, v);
            m_mesh.texcoords.push_back(texcoord);
        }
        else if (buffer[0] == 'f')
        {
            if (meshMark.isNewMesh)
            {
                std::string keyName{""};

                if (meshMark.s.compare("") != 0)
                {
                    keyName = meshMark.s;
                }

                if (meshMark.g.compare("") != 0)
                {
                    keyName = meshMark.g;
                }

                if (meshMark.usemtl.compare("") != 0)
                {
                    keyName = meshMark.usemtl;
                }

                m_mesh.faces.resize(++index);
                m_mesh.faces[index - 1].usemtl = keyName;

                meshMark.Clear();
            }

            if (mtllib->compare("") == 0)
            {
                *mtllib = "none_mtl";
            }

            if (m_mesh.faces.size() == 0)
            {
                m_mesh.faces.resize(1);
                m_mesh.faces[0].usemtl = "none_material";
            }

            // faces
            char linebuf[MaxLineLength];
            file.getline(linebuf, MaxLineLength);

            for (size_t k = strlen(linebuf) - 1; k >= 0; --k)
            {
                if (linebuf[k] == ' ')
                    linebuf[k] = '\0';
                else
                    break;
            }
            std::vector<uint32_t> vIndex, vtIndex, vnIndex;
            ParseLinebuf(linebuf, vIndex, vtIndex, vnIndex);

            if (vIndex.size() == vtIndex.size() && vIndex.size() == vnIndex.size())
            {
                if (3 == vIndex.size())
                {
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[0] - 1);
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[1] - 1);
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[2] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[0] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[1] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[2] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[0] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[1] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[2] - 1);
                }
                else if (4 == vIndex.size())
                {
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[0] - 1);
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[1] - 1);
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[2] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[0] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[1] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[2] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[0] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[1] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[2] - 1);

                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[2] - 1);
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[3] - 1);
                    m_mesh.faces[index - 1].pos_indices.push_back(vIndex[0] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[2] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[3] - 1);
                    m_mesh.faces[index - 1].tex_indices.push_back(vtIndex[0] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[2] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[3] - 1);
                    m_mesh.faces[index - 1].nor_indices.push_back(vnIndex[0] - 1);
                }
                else
                {
                    return;
                }
            }
        }
    }

    file.close();

    // clear indices
    for (auto &face : m_mesh.faces)
    {
        if (0 == m_mesh.positions.size())
            face.pos_indices.clear();

        if (0 == m_mesh.texcoords.size())
            face.tex_indices.clear();

        if (0 == m_mesh.normals.size())
            face.nor_indices.clear();
    }
}

void ObjReader::ParseMtl(const std::string &filename, const std::string &dir)
{
    std::cout << "mtl path: " << filename << std::endl;

    std::ifstream file(filename);
    if (!file)
    {
        return;
    }

    // some scratch memory
    char buffer[MaxLineLength];
    size_t index = 0;

    while (file)
    {
        file >> buffer;

        if (buffer[0] == 'o' || buffer[0] == '#')
        {
            // ignore smoothing groups, groups and objects
            char linebuf[MaxLineLength];
            file.getline(linebuf, MaxLineLength);
            // std::cout << "other Info: (" << buffer[0] << " " << linebuf << ")" << std::endl;
        }
        else if (strcmp(buffer, "newmtl") == 0)
        {
            // newmtl
            std::string buf;
            file >> buf;

            for (size_t i = 0; i < m_mesh.faces.size(); ++i)
            {
                if (0 == strcmp(buf.c_str(), m_mesh.faces[i].usemtl.c_str()))
                {
                    index = i;
                    break;
                }
            }
        }
        else if (strcmp(buffer, "map_Ka") == 0)
        {
            // map_Ka
            std::string buf;
            file >> buf;
            m_mesh.faces[index].textures.albedo.file_name = dir + buf;
        }
        else if (strcmp(buffer, "map_Kd") == 0)
        {
            // map_Kd
            std::string buf;
            file >> buf;
            m_mesh.faces[index].textures.albedo.file_name = dir + buf;
        }
        else if (strcmp(buffer, "map_Ks") == 0)
        {
            // map_Ks
            std::string buf;
            file >> buf;
            m_mesh.faces[index].textures.metallic.file_name = dir + buf;
        }
        // else if (strcmp(buffer, "map_d") == 0)
        //{
        //	// map_d
        //	std::string buf;
        //	file >> buf;
        //	_mesh.textureInfo[index - 1].map_d = prefixPath + buf;
        //}
        else if ((strcmp(buffer, "map_Bump") == 0) || (strcmp(buffer, "bump") == 0))
        {
            // map_bump
            std::string buf;
            file >> buf;
            m_mesh.faces[index].textures.normal.file_name = dir + buf;
        }
        else
        {
            ;
        }
    }

    file.close();
}

void ObjReader::ParseLinebuf(const char *linebuf, std::vector<uint32_t> &vIndex, std::vector<uint32_t> &vtIndex,
                             std::vector<uint32_t> &vnIndex)
{
    uint32_t faceIndexCount = 0;
    for (uint32_t i = 0; i < strlen(linebuf); ++i)
    {
        if (linebuf[i] == ' ')
            ++faceIndexCount;
    }
    vIndex.resize(faceIndexCount);
    vtIndex.resize(faceIndexCount);
    vnIndex.resize(faceIndexCount);

    std::istringstream is(linebuf);
    std::vector<uint32_t> data;
    char ch;
    while (is >> ch)
    {
        if (ch >= '0' && ch <= '9')
        {
            is.putback(ch);
            uint32_t i;
            is >> i;
            data.push_back(i);
        }
    }

    if ((int)data.size() / faceIndexCount == 1)
    {
        for (size_t i = 0; i < faceIndexCount; ++i)
        {
            vIndex[i] = data[i];
            vtIndex[i] = 0;
            vnIndex[i] = 0;
        }
    }
    else if ((int)data.size() / faceIndexCount == 2)
    {
        for (size_t i = 0; i < faceIndexCount; ++i)
        {
            vIndex[i] = data[i * 2];
            vtIndex[i] = data[i * 2 + 1];
            vnIndex[i] = 0;
        }
    }
    else if ((int)data.size() / faceIndexCount == 3)
    {
        for (size_t i = 0; i < faceIndexCount; ++i)
        {
            vIndex[i] = data[i * 3];
            vtIndex[i] = data[i * 3 + 1];
            vnIndex[i] = data[i * 3 + 2];
        }
    }
    else
    {
        return;
    }
}
