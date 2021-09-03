///////////////////////////////////////////////////////////////////////////////
// view_controller.h
// ===============
///////////////////////////////////////////////////////////////////////////////

#include "view_controllers/view_controller.h"
#include "camera.h"
#include <glm/gtx/euler_angles.hpp>

ViewController::ViewController()
{
}

ViewController::~ViewController()
{
    m_camera.reset();
}

void ViewController::SetCamera(const std::shared_ptr<Camera> camera_ptr)
{
    m_camera = camera_ptr;
}

void ViewController::SetViewPort(const int left, const int top, const int width, const int height)
{
    m_camera->SetViewPort(left, top, width, height);
}

void ViewController::SetMousePosition(const float x, const float y)
{
    m_mouse_x = x;
    m_mouse_y = y;
}
