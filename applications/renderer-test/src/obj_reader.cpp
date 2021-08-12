#include "obj_reader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace OBJ
{

const uint32_t MaxLineLength = 256;

std::string GetFileName(const char *path)
{
    std::string str(path);
    char ch1 = (char)'//';
    size_t pos1 = str.find_last_of(ch1);

    char ch2 = (char)'.';
    size_t pos2 = str.find_last_of(ch2);

    return str.substr(pos1 + 1, pos2 - pos1 - 1);
}

ObjReader::ObjReader(const std::string &filename, const std::string &dir)
{
    m_model_name = GetFileName(filename.c_str());

    ParseObj(filename, &m_model.mtllib);
    ParseMtl(dir + "/" + m_model.mtllib, dir);
}

bool ObjReader::IsSharedIndices()
{
    for (const auto &faces : m_model.faces)
    {
        const int count = faces.pos_indices.size();
        for (int i = 0; i < count; ++i)
        {
            if (faces.pos_indices[i] != faces.nor_indices[i] || faces.pos_indices[i] != faces.tex_indices[i])
            {
                return false;
            }
        }
    }
    return true;
}

// void ObjReader::GetMesh(const std::shared_ptr<GLMesh> mesh)
//{
//    glm::vec3 center;
//    float radius;
//    GLMath::ComputeBoundingSphereRitter(m_model.positions, &center, &radius);
//    mesh->SetBoundingBox(center, radius);
//
//    for (const auto &face : m_model.faces)
//    {
//        assert(face.pos_indices.size() == face.nor_indices.size());
//        assert(face.pos_indices.size() == face.tex_indices.size());
//
//        auto &parts_mesh = mesh->GetVariablePartsMeshes()[face.usemtl];
//
//        const int vertex_size = face.pos_indices.size();
//        parts_mesh.vertices.resize(vertex_size);
//        for (int i = 0; i < vertex_size; ++i)
//        {
//            parts_mesh.vertices[i].position = m_model.positions[face.pos_indices[i]];
//            parts_mesh.vertices[i].normal = m_model.normals[face.nor_indices[i]];
//            parts_mesh.vertices[i].texcoord = m_model.texcoords[face.tex_indices[i]];
//        }
//        GLMath::ComputeTangents(parts_mesh.vertices);
//
//        parts_mesh.textures = face.textures;
//        parts_mesh.material = face.material;
//    }
//}

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

    FaceMark face_mark;

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
            face_mark.is_new = true;
            file >> face_mark.g;
        }
        else if (strcmp(buffer, "s") == 0)
        {
            face_mark.is_new = true;
            file >> face_mark.s;
        }
        else if (strcmp(buffer, "usemtl") == 0)
        {
            face_mark.is_new = true;
            file >> face_mark.usemtl;
        }
        else if (strcmp(buffer, "v") == 0)
        {
            // positions
            float x, y, z;
            file >> x >> y >> z;
            glm::vec3 vertex(x * 0.1f, y * 0.1f, z * 0.1f);
            m_model.positions.push_back(vertex);
        }
        else if (strcmp(buffer, "vn") == 0)
        {
            // normals
            float x, y, z;
            file >> x >> y >> z;
            glm::vec3 normal(x, y, z);
            m_model.normals.push_back(normal);
        }
        else if (strcmp(buffer, "vt") == 0)
        {
            // texture coords
            float u, v;
            file >> u >> v;
            glm::vec2 texcoord(u, v);
            m_model.texcoords.push_back(texcoord);
        }
        else if (buffer[0] == 'f')
        {
            if (face_mark.is_new)
            {
                std::string keyName{""};

                if (face_mark.s.compare("") != 0)
                {
                    keyName = face_mark.s;
                }

                if (face_mark.g.compare("") != 0)
                {
                    keyName = face_mark.g;
                }

                if (face_mark.usemtl.compare("") != 0)
                {
                    keyName = face_mark.usemtl;
                }

                m_model.faces.resize(++index);
                m_model.faces[index - 1].usemtl = keyName;

                m_model.usemtles.resize(index);
                m_model.usemtles[index - 1].usemtl = keyName;

                face_mark.Clear();
            }

            if (mtllib->compare("") == 0)
            {
                *mtllib = "none_mtl";
            }

            if (m_model.faces.size() == 0)
            {
                m_model.faces.resize(1);
                m_model.faces[0].usemtl = "none_material";

                m_model.usemtles.resize(1);
                m_model.usemtles[0].usemtl = "none_material";
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
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[0] - 1);
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[1] - 1);
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[2] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[0] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[1] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[2] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[0] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[1] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[2] - 1);
                }
                else if (4 == vIndex.size())
                {
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[0] - 1);
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[1] - 1);
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[2] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[0] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[1] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[2] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[0] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[1] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[2] - 1);

                    m_model.faces[index - 1].pos_indices.push_back(vIndex[2] - 1);
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[3] - 1);
                    m_model.faces[index - 1].pos_indices.push_back(vIndex[0] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[2] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[3] - 1);
                    m_model.faces[index - 1].tex_indices.push_back(vtIndex[0] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[2] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[3] - 1);
                    m_model.faces[index - 1].nor_indices.push_back(vnIndex[0] - 1);
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
    for (auto &face : m_model.faces)
    {
        if (0 == m_model.positions.size())
            face.pos_indices.clear();

        if (0 == m_model.texcoords.size())
            face.tex_indices.clear();

        if (0 == m_model.normals.size())
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

            for (size_t i = 0; i < m_model.usemtles.size(); ++i)
            {
                if (0 == strcmp(buf.c_str(), m_model.usemtles[i].usemtl.c_str()))
                {
                    index = i;
                    break;
                }
            }
        }
        else if (strcmp(buffer, "Ns") == 0)
        {
            // Ns
            float ns;
            file >> ns;
            m_model.usemtles[index].ns = ns;
        }
        else if (strcmp(buffer, "Ka") == 0)
        {
            // Ka
            float x, y, z;
            file >> x >> y >> z;
            m_model.usemtles[index].ka = glm::vec3(x, y, z);
        }
        else if (strcmp(buffer, "Kd") == 0)
        {
            // Kd
            float x, y, z;
            file >> x >> y >> z;
            m_model.usemtles[index].kd = glm::vec3(x, y, z);
        }
        else if (strcmp(buffer, "Ks") == 0)
        {
            // Ks
            float x, y, z;
            file >> x >> y >> z;
            m_model.usemtles[index].ks = glm::vec3(x, y, z);
        }
        else if (strcmp(buffer, "Ni") == 0)
        {
            // Ni
            float ni;
            file >> ni;
            m_model.usemtles[index].ni = ni;
        }
        else if (strcmp(buffer, "d") == 0)
        {
            // d
            float d;
            file >> d;
            m_model.usemtles[index].d = d;
        }
        else if (strcmp(buffer, "illum") == 0)
        {
            // illum
            float illum;
            file >> illum;
            m_model.usemtles[index].illum = illum;
        }
        else if (strcmp(buffer, "map_Ka") == 0)
        {
            // map_Ka
            std::string buf;
            file >> buf;
            m_model.usemtles[index].map_kd = dir + "/" + buf;
        }
        else if (strcmp(buffer, "map_Kd") == 0)
        {
            // map_Kd
            std::string buf;
            file >> buf;
            m_model.usemtles[index].map_kd = dir + "/" + buf;
        }
        else if (strcmp(buffer, "map_Ks") == 0)
        {
            // map_Ks
            std::string buf;
            file >> buf;
            m_model.usemtles[index].map_ks = dir + "/" + buf;
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
            m_model.usemtles[index].map_bump = dir + "/" + buf;
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

} // namespace OBJ
