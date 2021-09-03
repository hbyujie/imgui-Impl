///////////////////////////////////////////////////////////////////////////////
// camera.cpp
// ===============
///////////////////////////////////////////////////////////////////////////////

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <stdio.h>

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::SetAngle(const float pitch, const float yaw, const float roll)
{
	glm::vec3 angle = glm::vec3(pitch, yaw, roll);
	this->SetAngle(angle);
}

void Camera::SetAngle(const glm::vec3 &angle)
{
    m_angle = glm::radians(angle);
}

void Camera::SetTarget(const glm::vec3 &target)
{
    m_target = target;
}

void Camera::SetDistance(const float distance)
{
	m_distance = distance;
}

void Camera::SetProjectionType(const ProjectionType type)
{
	m_projection_type = type;
}

void Camera::SetFov(const float fov)
{
    m_fov = fov;
}

void Camera::SetViewPort(const int left, const int top, const int width, const int height)
{
    m_view_port.left = static_cast<float>(left);
    m_view_port.top = static_cast<float>(top);
    m_view_port.width = static_cast<float>(width);
    m_view_port.height = static_cast<float>(height);
}

void Camera::SetViewPort(const ViewPort &view_port)
{
    m_view_port = view_port;
}

glm::vec3 Camera::GetPosition() const
{
    glm::mat4 rotate = EulerXYZToMat4InDegrees(m_angle);
  //  printf("rotate: \n%f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f.\n",
		//rotate[0][0], rotate[0][1], rotate[0][2], rotate[0][3],
		//rotate[1][0], rotate[1][1], rotate[1][2], rotate[1][3],
		//rotate[2][0], rotate[2][1], rotate[2][2], rotate[2][3],
		//rotate[3][0], rotate[3][1], rotate[3][2], rotate[3][3]);

    glm::vec3 forward = glm::vec3(-rotate[0][2], -rotate[1][2], -rotate[2][2]);
    glm::vec3 position = m_target - (m_distance * glm::normalize(forward));

    return position;
}

glm::mat4 Camera::GetView() const
{
	glm::vec3 position = this->GetPosition();
	glm::mat4 view = glm::lookAt(position, m_target, m_up);

    return view;
};

glm::mat4 Camera::GetProjection() const
{
    glm::mat4 projection;

    switch (m_projection_type)
    {
    case PERSPECTIVE: {
        projection = glm::perspective(m_fov, m_view_port.width / m_view_port.height, m_near_plane, m_far_plane);
        break;
    }
    case ORTHOGRAPHIC: {
        projection = glm::ortho(0.f, m_view_port.width, 0.f, m_view_port.height, m_near_plane, m_far_plane);
        break;
    }
    default:
        break;
    }

    return projection;
}

// glm::vec3 Camera::Mat4ToEulerXYZInDegrees(const glm::mat4 &mat4)
//{
//    auto view_rotate = glm::mat4(glm::mat3(mat4));
//
//    glm::vec3 angle;
//    glm::extractEulerAngleXYZ(view_rotate, angle.x, angle.y, angle.z);
//    angle = glm::degrees(angle);
//
//    return glm::vec3(angle.x, angle.y, angle.z);
//}

const glm::mat4 Camera::EulerXYZToMat4InDegrees(const glm::vec3 &angle) const
{
    glm::mat4 rotate_x = glm::transpose(glm::eulerAngleX(angle.x));
    glm::mat4 rotate_y = glm::transpose(glm::eulerAngleY(angle.y));
    glm::mat4 rotate_z = glm::transpose(glm::eulerAngleZ(angle.z));

    return glm::transpose(rotate_z * rotate_y * rotate_x);
}