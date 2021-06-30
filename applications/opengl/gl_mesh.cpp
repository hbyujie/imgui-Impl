#include "gl_mesh.h"
#include "image/stb_image.h"

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
		CreateTexture(textures.albedo);
    }

    file_name = textures.normal.file_name;
    if (!file_name.empty())
    {
        if (textures.normal.id != 0)
        {
            DeleteTexture(textures.normal);
        }
		CreateTexture(textures.normal);
    }

    file_name = textures.metallic.file_name;
    if (!file_name.empty())
    {
        if (textures.metallic.id != 0)
        {
            DeleteTexture(textures.metallic);
        }
		CreateTexture(textures.metallic);
    }

    file_name = textures.roughness.file_name;
    if (!file_name.empty())
    {
        if (textures.roughness.id != 0)
        {
            DeleteTexture(textures.roughness);
        }
		CreateTexture(textures.roughness);
    }

    file_name = textures.ao.file_name;
    if (!file_name.empty())
    {
        if (textures.ao.id != 0)
        {
            DeleteTexture(textures.ao);
        }
		CreateTexture(textures.ao);
    }
}

void GLMesh::CreateTexture(Texture &texture)
{
	if(texture.id != 0)
	{
		return;
	}

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(texture.is_vertical_flip);
	unsigned char *data = stbi_load(texture.file_name.c_str(), &width, &height, &nrComponents, 0);
	std::printf("Loading image: %s\n", texture.file_name.c_str());
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		texture.width = width;
		texture.height = height;

		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load image file: " + texture.file_name);
	}
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