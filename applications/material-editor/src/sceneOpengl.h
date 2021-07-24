
#pragma once

#include <GL/glew.h>
#include <QObject>
#include <unordered_set>
#include <glm/glm.hpp>
#include <memory>

class OrbitCamera;

class SceneOpengl : public QObject
{
  public:
    SceneOpengl(QObject *parent = nullptr);
    ~SceneOpengl() override;

    void Draw();

	void AddModel(const std::string &file_name);

	void SetViewPort(GLint x, GLint y, GLsizei width, GLsizei height);

	void SetViewCenterAndRadius(const glm::vec3& center, const float radius);

	void SetMousePosition(const float x, const float y);

	void RotateCamera(const float x, const float y);

	void MoveCamera(const float x, const float y);

	void ZoomCamera(const float delta);

private:
	void UpdateProjection(GLint x, GLint y, GLsizei width, GLsizei height);
	void UpdateView();

	void DrawLights();

  private:
    GLclampf m_bk_color[4]{178 / 255.f, 159 / 255.f, 250 / 255.f, 1.0f};

	std::unordered_set<std::string> m_mesh_file_names;
	
	OrbitCamera * m_camera;

	float m_mouse_x{ 0.f };
	float m_mouse_y{ 0.f };
	
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::vec3 m_eye_pos;
};
