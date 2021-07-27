
#pragma once

#include <GL/glew.h>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OrbitCamera.h"

#include "geometry.h"
#include "light.h"

class Shader;

class Scene
{
  public:
    Scene();
    ~Scene();

    void InitContext();

    void InitShaders();

    void InitTriangle();

    void SetViewPort(GLint x, GLint y, GLsizei width, GLsizei height);

    void SetViewCenterAndRadius(const glm::vec3 &center, const float radius);

    void AddGeometry(const std::string &name, const Geometry &geometry);

    void Draw();

    void SetMousePosition(const float x, const float y);

    void RotateCamera(const float x, const float y);

    void MoveCamera(const float x, const float y);

    void ZoomCamera(const float delta);

  private:
    void UpdateProjection();
    void DrawImgui();
    void DrawOpengl();

  private:
    float m_bk_color[3]{48 / 255.f, 48 / 255.f, 48 / 255.f};

    float m_view_width{1.0};
    float m_view_height{1.0};

    float m_fov{45.f};
    float m_near_plane{0.1f};
    float m_far_plane{100000.f};

    float m_mouse_x{0.f};
    float m_mouse_y{0.f};
    std::unique_ptr<OrbitCamera> m_camera{nullptr};

    GLuint m_ubo_matrices;

    // geometries
    GeometryMap m_geometries;

    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

    // light
    DirectLight m_direct_light;
};
