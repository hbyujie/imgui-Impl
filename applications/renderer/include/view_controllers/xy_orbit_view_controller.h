///////////////////////////////////////////////////////////////////////////////
// xy_orbit_view_controller.h
// ===============
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"
#include <memory>

class Camera;

class XYOrbitViewController
{
  public:
    XYOrbitViewController();
    ~XYOrbitViewController();

    void SetCamera(const std::shared_ptr<Camera> camera_ptr);

    void SetMousePosition(const float x, const float y);

    void RotateCamera(const float x, const float y);

    void MoveCamera(const float x, const float y);

    void ZoomCamera(const float delta);

  private:
    float m_mouse_x{0.f};
    float m_mouse_y{0.f};
    std::shared_ptr<Camera> m_camera{nullptr};
};
