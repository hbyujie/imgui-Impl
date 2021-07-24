
#pragma once

#include <memory>
#include <unordered_map>
#include <map>
#include <string>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OrbitCamera.h"

#include "geometry.h"
#include "light.h"

class Shader;

class Scene
{
public:
	Scene();
	~Scene();

	void InitContext();

	void InitShaders();

	void InitTriangle();
		
	void SetViewPort(GLint x, GLint y, GLsizei width, GLsizei height);

	void SetViewCenterAndRadius(const glm::vec3& center, const float radius);

	void AddGeometry(const std::string& name, const Geometry& geometry);

	void Draw();
	
	void SetMousePosition(const float x, const float y);

	void RotateCamera(const float x, const float y);

	void MoveCamera(const float x, const float y);

	void ZoomCamera(const float delta);

private:
	void DrawImgui();
	void DrawOpengl();

private:
	GLclampf m_bk_color[4]{ 178 / 255.f, 159 / 255.f, 250 / 255.f, 1.0f };

	float m_fov{ 45.f };
	float m_near_plane{ 0.1f };
	float m_far_plane{ 100000.f };

	float m_mouse_x{ 0.f };
	float m_mouse_y{ 0.f };
	std::unique_ptr<OrbitCamera> m_camera{ nullptr };

	GLuint m_ubo_matrices;

	// geometries
	GeometryMap m_geometry_map;

	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

	// light
	DirectLight m_direct_light;
};
