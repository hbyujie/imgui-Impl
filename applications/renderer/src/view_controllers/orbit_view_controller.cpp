///////////////////////////////////////////////////////////////////////////////
// orbit_view_controller.h
// ===============
///////////////////////////////////////////////////////////////////////////////

#include "view_controllers/orbit_view_controller.h"
#include "camera.h"
#include <glm/gtx/euler_angles.hpp>

OrbitViewController::OrbitViewController()
{
}

OrbitViewController::~OrbitViewController()
{
    m_camera.reset();
}

void OrbitViewController::Zero()
{
	if (m_camera == nullptr)
	{
		return;
	}

	m_camera->SetAngle(45.0f, 45.0f, 0.0f);
	m_camera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	m_camera->SetFov(45.0f);
	m_camera->SetProjectionType(ProjectionType::PERSPECTIVE);
	m_camera->SetDistance(10.0f);
}

void OrbitViewController::RotateCamera(const float x, const float y)
{
    glm::vec3 angle = m_camera->GetAngle();

    angle.y += (x - m_mouse_x) * 0.3f;
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

	m_camera->SetAngle(angle);

    m_mouse_x = x;
    m_mouse_y = y;
}

void OrbitViewController::MoveCamera(const float x, const float y)
{
    const float move_scale = m_camera->GetDistance() * 0.001f;

    glm::vec3 offset{glm::vec3(0.0)};
	offset.x = (x - m_mouse_x) * move_scale;
	offset.y = -(y - m_mouse_y) * move_scale;

	glm::mat4 view = m_camera->GetView();
	glm::vec3 cameraLeft(-view[0][0], -view[1][0], -view[2][0]);
	glm::vec3 cameraUp(-view[0][1], -view[1][1], -view[2][1]);

	glm::vec3 target = offset.x * cameraLeft + offset.y * cameraUp + m_camera->GetTarget();
    m_camera->SetTarget(target);

    m_mouse_x = x;
    m_mouse_y = y;
}

void OrbitViewController::ZoomCamera(const float delta)
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