
#include "simular_scene.h"
#include "stb/stb_image.h"

#include "shader.h"

SimularScene::SimularScene()
{
}

SimularScene::~SimularScene()
{
    for (auto &gl_primitive_map : m_gl_primitives)
    {
        DeleteOpenGLPrimitive(&gl_primitive_map.second);
    }
}

void SimularScene::AddPrimitive(const std::string &name, const Primitive &geometry)
{
    m_primitives[name] = geometry;

    this->CreateOpenGLPrimitive(geometry, &m_gl_primitives[name]);
}

void SimularScene::Draw(const std::shared_ptr<Shader> shader)
{
    shader->Bind();

    for (auto &gl_primitive_map : m_gl_primitives)
    {
        auto &gl_primitive = gl_primitive_map.second;

        shader->LinkUniformMat4("model", gl_primitive.model_matrix);

        glBindTextureUnit(0, gl_primitive.albedo);
        glBindTextureUnit(1, gl_primitive.normal);
        glBindTextureUnit(2, gl_primitive.metallic);
        glBindTextureUnit(3, gl_primitive.roughness);
        glBindTextureUnit(4, gl_primitive.ao);

        shader->LinkUniformBool("use_albedo_texture", false);
        shader->LinkUniformBool("use_normal_texture", false);
        shader->LinkUniformBool("use_metallic_texture", false);
        shader->LinkUniformBool("use_roughness_texture", false);
        shader->LinkUniformBool("use_ao_texture", false);

        if (gl_primitive.albedo != 0)
        {
            shader->LinkUniformBool("use_albedo_texture", true);
        }

        if (gl_primitive.normal != 0)
        {
            shader->LinkUniformBool("use_normal_texture", true);
        }

        if (gl_primitive.metallic != 0)
        {
            shader->LinkUniformBool("use_metallic_texture", true);
        }

        glLineWidth(gl_primitive.line_width);
        glPointSize(gl_primitive.point_size);

        glBindVertexArray(gl_primitive.vao);
        glDrawElements(gl_primitive.mode, gl_primitive.count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

		glBindTextureUnit(0, 0);
		glBindTextureUnit(1, 0);
		glBindTextureUnit(2, 0);
		glBindTextureUnit(3, 0);
		glBindTextureUnit(4, 0);
    }

    shader->Release();
}

void SimularScene::CreateOpenGLPrimitive(const Primitive &primitive, OpenGL_Primitive *gl_primitive)
{
    auto &positions = primitive.positions;
    auto &normals = primitive.normals;
    auto &texcoords = primitive.texcoords;
    auto &colors = primitive.colors;
    auto &tangents = primitive.tangents;
    auto &bitangents = primitive.bitangents;
    auto &indices = primitive.indices;

    if (positions.size() == 0 || indices.size() == 0)
    {
        return;
    }

    glGenVertexArrays(1, &gl_primitive->vao);
    glBindVertexArray(gl_primitive->vao);

    // indices
    glGenBuffers(1, &gl_primitive->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_primitive->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
    gl_primitive->count = static_cast<GLsizei>(indices.size());

    // positions
    glGenBuffers(1, &gl_primitive->vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, gl_primitive->vbo_position);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // normals
    if (0 != normals.size())
    {
        glGenBuffers(1, &gl_primitive->vbo_normal);
        glBindBuffer(GL_ARRAY_BUFFER, gl_primitive->vbo_normal);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STREAM_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (void *)0);
    }

    // texcoords
    if (0 != texcoords.size())
    {
        glGenBuffers(1, &gl_primitive->vbo_texcoord);
        glBindBuffer(GL_ARRAY_BUFFER, gl_primitive->vbo_texcoord);
        glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(glm::vec2), &texcoords[0], GL_STREAM_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    // colors
    if (0 != colors.size())
    {
        glGenBuffers(1, &gl_primitive->vbo_color);
        glBindBuffer(GL_ARRAY_BUFFER, gl_primitive->vbo_color);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STREAM_DRAW);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    // tangents
    if (0 != tangents.size())
    {
        glGenBuffers(1, &gl_primitive->vbo_tangent);
        glBindBuffer(GL_ARRAY_BUFFER, gl_primitive->vbo_tangent);
        glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STREAM_DRAW);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    // bitangents
    if (0 != bitangents.size())
    {
        glGenBuffers(1, &gl_primitive->vbo_bitangent);
        glBindBuffer(GL_ARRAY_BUFFER, gl_primitive->vbo_bitangent);
        glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STREAM_DRAW);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    gl_primitive->line_width = primitive.line_width;
    gl_primitive->point_size = primitive.point_size;

    gl_primitive->mode = primitive.mode;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    gl_primitive->albedo = this->LoadTexture(primitive.albedo_file);
    gl_primitive->normal = this->LoadTexture(primitive.normal_file);
    gl_primitive->metallic = this->LoadTexture(primitive.metallic_file);
    gl_primitive->roughness = this->LoadTexture(primitive.roughness_file);
    gl_primitive->ao = this->LoadTexture(primitive.ao_file);
}

void SimularScene::DeleteOpenGLPrimitive(OpenGL_Primitive *gl_primitive)
{
    if (gl_primitive->vao)
    {
        glDeleteVertexArrays(1, &gl_primitive->vao);
    }

    if (gl_primitive->ebo)
    {
        glDeleteBuffers(1, &gl_primitive->ebo);
    }

    if (gl_primitive->vbo_position)
    {
        glDeleteBuffers(1, &gl_primitive->vbo_position);
    }
    if (gl_primitive->vbo_normal)
    {
        glDeleteBuffers(1, &gl_primitive->vbo_normal);
    }
    if (gl_primitive->vbo_texcoord)
    {
        glDeleteBuffers(1, &gl_primitive->vbo_texcoord);
    }
    if (gl_primitive->vbo_color)
    {
        glDeleteBuffers(1, &gl_primitive->vbo_color);
    }
    if (gl_primitive->vbo_tangent)
    {
        glDeleteBuffers(1, &gl_primitive->vbo_tangent);
    }
    if (gl_primitive->vbo_bitangent)
    {
        glDeleteBuffers(1, &gl_primitive->vbo_bitangent);
    }

    if (gl_primitive->albedo)
    {
        glDeleteTextures(1, &gl_primitive->albedo);
    }
    if (gl_primitive->normal)
    {
        glDeleteTextures(1, &gl_primitive->normal);
    }
    if (gl_primitive->metallic)
    {
        glDeleteTextures(1, &gl_primitive->metallic);
    }
    if (gl_primitive->roughness)
    {
        glDeleteTextures(1, &gl_primitive->roughness);
    }
    if (gl_primitive->ao)
    {
        glDeleteTextures(1, &gl_primitive->ao);
    }

    std::memset(gl_primitive, 0, sizeof(OpenGL_Primitive));
}

GLuint SimularScene::LoadTexture(const std::string &path, const bool is_vertical_flip)
{
    GLuint textureID{0};

    if (path.empty())
    {
        return textureID;
    }

    glGenTextures(1, &textureID);

    int width, height, nrComponents;

	stbi_set_flip_vertically_on_load(is_vertical_flip);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        printf("Texture failed to load at path: %s.\n", path.c_str());
        stbi_image_free(data);
    }

    return textureID;
}