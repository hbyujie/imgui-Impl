///////////////////////////////////////////////////////////////////////////////
// view_controller.h
// ===============
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "glm/glm.hpp"
#include <memory>

class Camera;

class ViewController
{
  public:
    ViewController();
    virtual ~ViewController();

    void SetCamera(const std::shared_ptr<Camera> camera_ptr);

    void SetViewPort(const int left, const int top, const int width, const int height);

    void SetMousePosition(const float x, const float y);

    virtual void Zero() = 0;

    virtual void RotateCamera(const float x, const float y) = 0;

    virtual void MoveCamera(const float x, const float y) = 0;

    virtual void ZoomCamera(const float delta) = 0;

  protected:
    float m_mouse_x{0.f};
    float m_mouse_y{0.f};
    std::shared_ptr<Camera> m_camera{nullptr};
};
