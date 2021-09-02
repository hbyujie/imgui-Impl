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

    void LookAt(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f));
    void SetViewPort(const int left, const int top, const int width, const int height);
	void SetViewPort(const ViewPort& view_port);
	void SetProjectionType(const ProjectionType type);

    void RotateToAngle(const glm::vec3 &angle);
    void Shift(const glm::vec3 &offset);
    void SetDistance(const float distance);

    const glm::vec3 GetAngle() const { return m_angle; }
	const float GetDistance() const { return m_distance; }
	const glm::mat4 GetView() const { return m_view; };

	const float GetNear() const { return m_near_plane; }
	const float GetFar() const { return m_far_plane; }	
	const glm::mat4 GetProjection() const { return m_projection; }

  private:
    glm::vec3 Mat4ToEulerXYZInDegrees(const glm::mat4 &mat4);
    glm::mat4 EulerXYZToMat4InDegrees(const glm::vec3 &angle);

  private:
    // member variables
    glm::vec3 m_up{glm::vec3(0.0f, 1.0f, 0.0f)};

    glm::vec3 m_position;   // camera position at world space
    glm::vec3 m_target;     // camera focal(lookat) position at world space
    glm::vec3 m_angle;      // angle in degree around the target (pitch, yaw, roll)
    float m_distance{0.0f}; // distance between position and target

    glm::mat4 m_view{glm::mat4(0.0)};

    float m_fov{45.f};
    float m_near_plane{0.1f};
    float m_far_plane{100.f};

	ViewPort m_view_port;
    ProjectionType m_projection_type{PERSPECTIVE};
    glm::mat4 m_projection{glm::mat4(0.0)};
};
