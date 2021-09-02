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

void Camera::LookAt(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up)
{
    m_position = position;
    m_target = target;
    m_up = up;

    m_distance = glm::length(m_position - m_target);
    m_view = glm::lookAt(m_position, m_target, m_up);
    m_angle = Mat4ToEulerXYZInDegrees(this->GetView());
}

void Camera::SetViewPort(const int left, const int top, const int width, const int height)
{
    m_view_port.left = static_cast<float>(left);
    m_view_port.top = static_cast<float>(top);
    m_view_port.width = static_cast<float>(width);
    m_view_port.height = static_cast<float>(height);

    this->SetViewPort(m_view_port);
}

void Camera::SetViewPort(const ViewPort &view_port)
{
    switch (m_projection_type)
    {
    case PERSPECTIVE: {
        m_projection = glm::perspective(m_fov, m_view_port.width / m_view_port.height, m_near_plane, m_far_plane);
        break;
    }
    case ORTHOGRAPHIC: {
        m_projection = glm::ortho(0.f, m_view_port.width, 0.f, m_view_port.height,  m_near_plane, m_far_plane);
        break;
    }
    default:
        break;
    }
}

void Camera::SetProjectionType(const ProjectionType type)
{
    m_projection_type = type;
}

void Camera::RotateToAngle(const glm::vec3 &angle)
{
    m_angle = angle;

    glm::mat4 view_rotate = EulerXYZToMat4InDegrees(angle);
    // printf("rotate: \n%f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f, \n%f, %f, %f, %f.\n",
    //	rotate[0][0], rotate[0][1], rotate[0][2], rotate[0][3],
    //	rotate[1][0], rotate[1][1], rotate[1][2], rotate[1][3],
    //	rotate[2][0], rotate[2][1], rotate[2][2], rotate[2][3],
    //	rotate[3][0], rotate[3][1], rotate[3][2], rotate[3][3]);

    // re-compute camera position
    glm::vec3 forward = glm::vec3(-view_rotate[0][2], -view_rotate[1][2], -view_rotate[2][2]);
    m_position = m_target - (m_distance * glm::normalize(forward));
    m_view = glm::lookAt(m_position, m_target, m_up);
}

void Camera::Shift(const glm::vec3 &offset)
{
    // get left & up vectors of camera
    glm::vec3 cameraLeft(-m_view[0][0], -m_view[1][0], -m_view[2][0]);
    glm::vec3 cameraUp(-m_view[0][1], -m_view[1][1], -m_view[2][1]);

    // compute delta movement
    glm::vec3 deltaMovement = offset.x * cameraLeft;
    deltaMovement += -offset.y * cameraUp; // reverse up direction

    m_target = m_target + deltaMovement; // find new target position

    // re-compute camera position
    glm::vec3 forward = glm::vec3(-m_view[0][2], -m_view[1][2], -m_view[2][2]);
    m_position = m_target - (m_distance * glm::normalize(forward));
    m_view = glm::lookAt(m_position, m_target, m_up);
}

void Camera::SetDistance(const float distance)
{
    m_distance = distance;

    // re-compute camera position
    glm::vec3 forward = glm::vec3(-m_view[0][2], -m_view[1][2], -m_view[2][2]);
    m_position = m_target - (m_distance * glm::normalize(forward));
    m_view = glm::lookAt(m_position, m_target, m_up);
}

glm::vec3 Camera::Mat4ToEulerXYZInDegrees(const glm::mat4 &mat4)
{
    auto view_rotate = glm::mat4(glm::mat3(mat4));

    glm::vec3 angle;
    glm::extractEulerAngleXYZ(view_rotate, angle.x, angle.y, angle.z);
    angle = glm::degrees(angle);

    return glm::vec3(angle.x, -angle.y, angle.z);
}

glm::mat4 Camera::EulerXYZToMat4InDegrees(const glm::vec3 &angle)
{
    auto arc_angle = glm::radians(angle);

    glm::mat4 rotate_x = glm::transpose(glm::eulerAngleX(arc_angle.x));
    glm::mat4 rotate_y = glm::transpose(glm::eulerAngleY(-arc_angle.y));
    glm::mat4 rotate_z = glm::transpose(glm::eulerAngleZ(arc_angle.z));

    return glm::transpose(rotate_z * rotate_y * rotate_x);
}