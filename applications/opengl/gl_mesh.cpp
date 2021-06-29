#include "gl_mesh.h"
#include "image/Image.hpp"

GLMesh::GLMesh()
{
}

GLMesh::~GLMesh()
{
    for (auto &part_mesh : m_parts_meshes)
    {
		DeleteVertex(part_mesh.second);

        DeleteTextures(part_mesh.second.textures);
    }
}

void GLMesh::SetName(const std::string &name)
{
    m_name = name;
}

void GLMesh::SetBoundingBox(const glm::vec3 center, const float radius)
{
    m_bounding_box_center = center;
    m_bounding_box_radius = radius;
}

void GLMesh::SetShaderName(const std::string shader_name)
{
    m_shader_name = shader_name;
}

void GLMesh::Upload()
{
    for (auto &part_mesh : m_parts_meshes)
    {
        UploadVertex(part_mesh.second);

        UploadTextures(part_mesh.second.textures);
    }
}

void GLMesh::UploadVertex(PartsMesh &parts_mesh)
{
    // vao
    glGenVertexArrays(1, &parts_mesh.vao);
    glBindVertexArray(parts_mesh.vao);

    // vbo
    glGenBuffers(1, &parts_mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, parts_mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, parts_mesh.vertices.size() * sizeof(Vertex), &parts_mesh.vertices[0], GL_STREAM_DRAW);

    // set attribPointer
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));
    // vertex colors
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));
    // vertex tangents
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
    // vertex bitangents
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLMesh::UploadTextures(Textures &textures)
{
    std::string file_name;

    file_name = textures.albedo.file_name;
    if (!file_name.empty())
    {
        if (textures.albedo.id != 0)
        {
            DeleteTexture(textures.albedo);
        }
        textures.albedo =
            CreateTexture(Image::fromFile(file_name, 3, textures.albedo.is_vertical_flip), GL_RGB, GL_RGB8, 0);
        textures.albedo.file_name = file_name;
    }

    file_name = textures.normal.file_name;
    if (!file_name.empty())
    {
        if (textures.normal.id != 0)
        {
            DeleteTexture(textures.normal);
        }
        textures.normal =
            CreateTexture(Image::fromFile(file_name, 3, textures.normal.is_vertical_flip), GL_RGB, GL_RGB8, 0);
        textures.normal.file_name = file_name;
    }

    file_name = textures.metallic.file_name;
    if (!file_name.empty())
    {
        if (textures.metallic.id != 0)
        {
            DeleteTexture(textures.metallic);
        }
        textures.metallic =
            CreateTexture(Image::fromFile(file_name, 3, textures.metallic.is_vertical_flip), GL_RED, GL_R8, 0);
        textures.metallic.file_name = file_name;
    }

    file_name = textures.roughness.file_name;
    if (!file_name.empty())
    {
        if (textures.roughness.id != 0)
        {
            DeleteTexture(textures.roughness);
        }
        textures.roughness =
            CreateTexture(Image::fromFile(file_name, 3, textures.roughness.is_vertical_flip), GL_RED, GL_R8, 0);
        textures.roughness.file_name = file_name;
    }

    file_name = textures.ao.file_name;
    if (!file_name.empty())
    {
        if (textures.ao.id != 0)
        {
            DeleteTexture(textures.ao);
        }
        textures.ao = CreateTexture(Image::fromFile(file_name, 3, textures.ao.is_vertical_flip), GL_RED, GL_R8, 0);
        textures.ao.file_name = file_name;
    }
}

Texture GLMesh::CreateTexture(GLenum target, int width, int height, GLenum internalformat, int levels) const
{
    Texture texture;
    texture.width = width;
    texture.height = height;
    texture.levels = (levels > 0) ? levels : NumMipmapLevels(width, height);

    glCreateTextures(target, 1, &texture.id);
    glTextureStorage2D(texture.id, texture.levels, internalformat, width, height);
    glTextureParameteri(texture.id, GL_TEXTURE_MIN_FILTER, texture.levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTextureParameteri(texture.id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameterf(texture.id, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_max_anisotropy);
    return texture;
}

Texture GLMesh::CreateTexture(const std::shared_ptr<class Image> &image, GLenum format, GLenum internalformat,
                              int levels) const
{
    if (image == nullptr)
    {
        return Texture();
    }

    Texture texture = CreateTexture(GL_TEXTURE_2D, image->width(), image->height(), internalformat, levels);
    if (image->isHDR())
    {
        glTextureSubImage2D(texture.id, 0, 0, 0, texture.width, texture.height, format, GL_FLOAT,
                            image->pixels<float>());
    }
    else
    {
        glTextureSubImage2D(texture.id, 0, 0, 0, texture.width, texture.height, format, GL_UNSIGNED_BYTE,
                            image->pixels<unsigned char>());
    }

    if (texture.levels > 1)
    {
        glGenerateTextureMipmap(texture.id);
    }
    return texture;
}

int GLMesh::NumMipmapLevels(int width, int height) const
{
    int levels = 1;
    while ((width | height) >> levels)
    {
        ++levels;
    }
    return levels;
}

void GLMesh::DeleteVertex(PartsMesh &parts_mesh)
{
    if (parts_mesh.vao)
    {
        glDeleteVertexArrays(1, &parts_mesh.vao);
    }
    if (parts_mesh.vbo)
    {
        glDeleteBuffers(1, &parts_mesh.vbo);
    }
}

void GLMesh::DeleteTextures(Textures &textures)
{
    if (textures.albedo.id != 0)
    {
        DeleteTexture(textures.albedo);
    }

    if (textures.normal.id != 0)
    {
        DeleteTexture(textures.normal);
    }

    if (textures.metallic.id != 0)
    {
        DeleteTexture(textures.metallic);
    }

    if (textures.roughness.id != 0)
    {
        DeleteTexture(textures.roughness);
    }

    if (textures.ao.id != 0)
    {
        DeleteTexture(textures.ao);
    }
}

void GLMesh::DeleteTexture(Texture &texture)
{
    glDeleteTextures(1, &texture.id);
    std::memset(&texture, 0, sizeof(Texture));
}