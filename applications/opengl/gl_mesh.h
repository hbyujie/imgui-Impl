#pragma once

#include "opengl/gl_defines.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>

class GLTextureManager;

class GLMesh
{
  public:
    GLMesh();
    ~GLMesh();

    void SetName(const std::string &name);

    void SetBoundingBox(const glm::vec3 center, const float radius);

    void SetShaderName(const std::string shader_name);

    void Upload();

    std::unordered_map<std::string, PartsMesh> &GetVariablePartsMeshes()
    {
        return m_parts_meshes;
    }

    const std::unordered_map<std::string, PartsMesh> &GetConstPartsMeshes()
    {
        return m_parts_meshes;
    }

    const std::string &GetShaderName()
    {
        return m_shader_name;
    }

    const glm::mat4 &GetModelMatrix()
    {
        return m_model_matrix;
    }

    const glm::vec3 &GetBoundingBoxCenter()
    {
        return m_bounding_box_center;
    }

    const float GetBoundingBoxRadius()
    {
        return m_bounding_box_radius;
    }

  private:
    void UploadVertex(PartsMesh &parts_mesh);

    void UploadTextures(Textures &textures);

    Texture CreateTexture(const std::shared_ptr<class Image> &image, GLenum format, GLenum internalformat,
                          int levels = 0) const;

    Texture CreateTexture(GLenum target, int width, int height, GLenum internalformat, int levels = 0) const;

    int NumMipmapLevels(int width, int height) const;

    void DeleteVertex(PartsMesh &parts_mesh);

    void DeleteTextures(Textures &textures);

    void DeleteTexture(Texture &texture);

  private:
    std::string m_name;
    std::string m_shader_name;
    glm::vec3 m_bounding_box_center;
    float m_bounding_box_radius;
    glm::mat4 m_model_matrix{glm::mat4(1.0f)};
    std::unordered_map<std::string, PartsMesh> m_parts_meshes;

    float m_max_anisotropy;
};