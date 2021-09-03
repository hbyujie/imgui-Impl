///////////////////////////////////////////////////////////////////////////////
// camera.h
// ===============
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

enum ProjectionType
{
    PERSPECTIVE = 0,
    ORTHOGRAPHIC = 1
};

struct ViewPort
{
    float left{0.0};
    float top{0.0};
    float width{1.0};
    float height{1.0};
};

class Camera
{
  public:
    Camera();
    ~Camera();
	
	// angle in degrees
	void SetAngle(const float pitch, const float yaw, const float roll);
	void SetAngle(const glm::vec3& angle);
	void SetTarget(const glm::vec3& target);
	void SetDistance(const float distance);
	void SetProjectionType(const ProjectionType type);
	void SetFov(const float fov);
	void SetViewPort(const int left, const int top, const int width, const int height);
	void SetViewPort(const ViewPort& view_port);
	
	const glm::vec3 GetTarget() const { return m_target; }
    const glm::vec3 GetAngle() const { return glm::degrees(m_angle); }
	const float GetDistance() const { return m_distance; }

	const float GetNear() const { return m_near_plane; }
	const float GetFar() const { return m_far_plane; }

	glm::vec3 GetPosition() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;

  private:
    //glm::vec3 Mat4ToEulerXYZInDegrees(const glm::mat4 &mat4);
    const glm::mat4 EulerXYZToMat4InDegrees(const glm::vec3 &angle) const;

  private:
    glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};

    glm::vec3 m_target;     // camera focal(lookat) position at world space
    glm::vec3 m_angle;      // angle in radians around the target (pitch, yaw, roll)
    float m_distance{0.0f}; // distance between position and target

    float m_fov{45.f};
    float m_near_plane{0.1f};
    float m_far_plane{100.f};

    ViewPort m_view_port;
    ProjectionType m_projection_type{PERSPECTIVE};
};
