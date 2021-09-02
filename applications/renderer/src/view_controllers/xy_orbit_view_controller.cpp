///////////////////////////////////////////////////////////////////////////////
// xy_orbit_view_controller.h
// ===============
///////////////////////////////////////////////////////////////////////////////

#include "view_controllers/xy_orbit_view_controller.h"
#include "camera.h"
#include <glm/gtx/euler_angles.hpp>

XYOrbitViewController::XYOrbitViewController()
{
}

XYOrbitViewController::~XYOrbitViewController()
{
    m_camera.reset();
}

void XYOrbitViewController::SetCamera(const std::shared_ptr<Camera> camera_ptr)
{
    m_camera = camera_ptr;
	//m_camera->SetProjectionType(ProjectionType::ORTHOGRAPHIC);
}

void XYOrbitViewController::SetMousePosition(const float x, const float y)
{
    m_mouse_x = x;
    m_mouse_y = y;
}

void XYOrbitViewController::RotateCamera(const float x, const float y)
{
    glm::vec3 angle = m_camera->GetAngle();

    angle.y -= (x - m_mouse_x) * 0.3f;
    angle.x += (y - m_mouse_y) * 0.3f;

    // constrain x angle -89 < x < 89
    if (angle.x < -89.0f)
    {
        angle.x = -89.0f;
    }
    else if (angle.x > 89.0f)
    {
        angle.x = 89.0f;
    }

    m_camera->RotateToAngle(angle);

    m_mouse_x = x;
    m_mouse_y = y;
}

void XYOrbitViewController::MoveCamera(const float x, const float y)
{
    const float move_scale = m_camera->GetDistance() * 0.001f;

    glm::vec3 delta_move{glm::vec3(0.0)};
    delta_move.x = (x - m_mouse_x) * move_scale;
    delta_move.y = (y - m_mouse_y) * move_scale;

    m_camera->Shift(delta_move);

    m_mouse_x = x;
    m_mouse_y = y;
}

void XYOrbitViewController::ZoomCamera(const float delta)
{
    int sign = static_cast<int>(delta / fabs(delta));
    float delta_distance = sign * m_camera->GetDistance() * 0.1f;

    float distance = m_camera->GetDistance() + delta_distance;

    if (distance < m_camera->GetNear())
    {
        distance = m_camera->GetNear();
    }
    else if (distance > m_camera->GetFar())
    {
        distance = m_camera->GetFar();
    }

    m_camera->SetDistance(distance);
}