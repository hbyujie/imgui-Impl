
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scene.h"
#include "shader.h"
#include "sky_box.h"

#include <imgui.h>

Scene::Scene()
{
}

Scene::~Scene()
{
    m_camera.reset();

    for (auto &geometry_map : m_geometries)
    {
        geometry_map.second.DeletePrimitiveBuffers();
        geometry_map.second.DeletePrimitiveTextures();
    }
    m_geometries.clear();


	glDeleteBuffers(1, &m_ubo_matrices);

	glDeleteBuffers(1, &m_shadow_texture.frame_buffer);
	glDeleteTextures(1, &m_shadow_texture.texture);
}

void Scene::InitContext()
{
    glGenBuffers(1, &m_ubo_matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo_matrices, 0, 2 * sizeof(glm::mat4));

    m_shaders["Base_Shader"].reset(
        new Shader(QString(":/shaders/shader/base.vs"), QString(":/shaders/shader/base.fs")));
    m_shaders["Base_Shader"]->LinkUniformBlock("Matrices", 0);

    m_shaders["BlinnPhong"].reset(
        new Shader(QString(":/shaders/shader/blinnPhong.vs"), QString(":/shaders/shader/blinnPhong.fs")));
    m_shaders["BlinnPhong"]->LinkUniformBlock("Matrices", 0);

    m_shaders["SkyBox"].reset(new Shader(QString(":/shaders/shader/skybox.vs"), QString(":/shaders/shader/skybox.fs")));
    m_shaders["SkyBox"]->LinkUniformBlock("Matrices", 0);

	m_shaders["DepthMap"].reset(new Shader(QString(":/shaders/shader/depth_map.vs"), QString(":/shaders/shader/depth_map.fs")));

    m_camera.reset(new OrbitCamera());

    m_sky_box.reset(new SkyBox());
    m_sky_box->SetName("SkyBox");
    m_sky_box->SetShader(m_shaders["SkyBox"]);
    m_sky_box->SetTextures({
        QString("D:/imgui-openglwidget/data/textures/skybox/right.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/left.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/top.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/bottom.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/front.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/back.jpg"),
    });

	{
		glGenFramebuffers(1, &m_shadow_texture.frame_buffer);

		glGenTextures(1, &m_shadow_texture.texture);
		glBindTexture(GL_TEXTURE_2D, m_shadow_texture.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			m_shadow_texture.width, m_shadow_texture.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_texture.frame_buffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_texture.texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Shadow Framebuffer is not complete!" << std::endl;
		}
	}

    m_direct_light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    m_direct_light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    m_direct_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    m_direct_light.direction = glm::vec3(3.0f, 4.0f, 5.0f);
    m_direct_light.enabled = true;
}

void Scene::InitShaders()
{
}

void Scene::InitTriangle()
{
    std::vector<glm::vec3> positions;
    positions.push_back(glm::vec3(-100, 100, 0));
    positions.push_back(glm::vec3(0, 100, 0));
    positions.push_back(glm::vec3(100, 100, 0));
    positions.push_back(glm::vec3(-100, 0, 0));
    positions.push_back(glm::vec3(0, 0, 0));
    positions.push_back(glm::vec3(100, 0, 0));
    positions.push_back(glm::vec3(-100, -100, 0));
    positions.push_back(glm::vec3(0, -100, 0));
    positions.push_back(glm::vec3(100, -100, 0));

    std::unordered_map<std::string, std::vector<uint32_t>> primitives;

    primitives["mesh_01"] = std::vector<uint32_t>();
    primitives["mesh_01"].push_back(0);
    primitives["mesh_01"].push_back(4);
    primitives["mesh_01"].push_back(3);
    primitives["mesh_01"].push_back(0);
    primitives["mesh_01"].push_back(1);
    primitives["mesh_01"].push_back(4);

    primitives["mesh_02"] = std::vector<uint32_t>();
    primitives["mesh_02"].push_back(3);
    primitives["mesh_02"].push_back(4);
    primitives["mesh_02"].push_back(7);
    primitives["mesh_02"].push_back(4);
    primitives["mesh_02"].push_back(8);
    primitives["mesh_02"].push_back(7);
}

void Scene::SetViewPort(GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewport(x, y, width, height);

    m_view_width = static_cast<float>(width);
    m_view_height = static_cast<float>(height);

    UpdateProjection();
}

void Scene::SetViewCenterAndRadius(const glm::vec3 &center, const float radius)
{
    Vector3 m_view_center = Vector3(center[0], center[1], center[2]);

    float scale = 3.9f;
    Vector3 eye = m_view_center + Vector3(0.f, 0.f, 1.f) * scale * radius;
    m_camera->lookAt(eye, m_view_center);
}

void Scene::AddGeometry(const std::string &name, const Geometry &geometry)
{
    m_geometries[name] = geometry;
    m_geometries[name].CreatePrimitiveBuffers();
    m_geometries[name].CreatePrimitiveTextures();
}

void Scene::Draw()
{
    //DrawImgui();
    DrawOpengl();
}

void Scene::SetMousePosition(const float x, const float y)
{
    m_mouse_x = x;
    m_mouse_y = y;
}

void Scene::RotateCamera(const float x, const float y)
{
    const float ANGLE_SCALE = 0.3f;
    Vector3 angle = m_camera->getAngle();
    angle.y -= (x - m_mouse_x) * ANGLE_SCALE;
    angle.x += (y - m_mouse_y) * ANGLE_SCALE;
    m_mouse_x = x;
    m_mouse_y = y;

    // constrain x angle -89 < x < 89
    if (angle.x < -89.0f)
        angle.x = -89.0f;
    else if (angle.x > 89.0f)
        angle.x = 89.0f;

    m_camera->rotateTo(angle);
}

void Scene::MoveCamera(const float x, const float y)
{
    const float MOVE_SCALE = m_camera->getDistance() * 0.001f;
    Vector2 shift_delta;
    shift_delta.x = (x - m_mouse_x) * MOVE_SCALE;
    shift_delta.y = (y - m_mouse_y) * MOVE_SCALE;
    m_mouse_x = x;
    m_mouse_y = y;

    m_camera->shift(shift_delta);
}

void Scene::ZoomCamera(const float delta)
{
    int sign = delta / fabs(delta);

    const float ZOOM_SCALE = m_camera->getDistance() * 0.1f;
    const float MIN_DIST = 0.1f;
    const float MAX_DIST = 100000.f;
    float distance = m_camera->getDistance();
    distance += (sign * ZOOM_SCALE);

    // constrain min and max
    if (distance < MIN_DIST)
        distance = MIN_DIST;
    else if (distance > MAX_DIST)
        distance = MAX_DIST;

    m_camera->setDistance(distance);
}

void Scene::UpdateProjection()
{
    glm::mat4 projection = glm::perspective(m_fov, m_view_width / m_view_height, m_near_plane, m_far_plane);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Scene::DrawImgui()
{
    static bool show_demo_window = true;
    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    bool show_displays = true;
    if (show_displays)
    {
        ImGui::Begin("Displays", &show_displays);

        if (ImGui::CollapsingHeader("Global Options"))
        {
            ImGui::ColorEdit3("Backgroud Color", m_bk_color);
        }

        for (const auto &geometry_map : m_geometries)
        {
            if (ImGui::CollapsingHeader(geometry_map.first.c_str()))
            {
            }
        }

        ImGui::End();

        ImGui::Begin("Views", &show_displays);

        if (ImGui::CollapsingHeader("Current View"))
        {
            if ((ImGui::InputFloat("Fov", &m_fov)) || (ImGui::InputFloat("Near Clip Distance", &m_near_plane)) ||
                (ImGui::InputFloat("Far Clip Distance", &m_far_plane)))
            {
                UpdateProjection();
            }
        }

        ImGui::End();
    }
}

void Scene::DrawOpengl()
{
	//// 1. Render depth of scene to texture (from light's perspective)
	//// - Get light projection/view matrix.
	//glm::mat4 lightProjection, lightView;
	//glm::mat4 light_projection_view;
	//glm::vec3 light_position = m_direct_light.direction;
	//GLfloat near_plane = 1.0f, far_plane = 7.5f;

	//lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	////lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
	//lightView = glm::lookAt(light_position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	//light_projection_view = lightProjection * lightView;

	//// - now render scene from light's point of view
	//m_shaders["DepthMap"]->Bind();
	//m_shaders["DepthMap"]->LinkUniformMat4("light_projection_view", light_projection_view);
	//m_shaders["DepthMap"]->Release();

	//glViewport(0, 0, m_shadow_texture.width, m_shadow_texture.height);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_texture.frame_buffer);
	//glClear(GL_DEPTH_BUFFER_BIT);

	//for (auto &geometry_map : m_geometries)
	//{
	//	geometry_map.second.Draw(m_shaders["DepthMap"]);
	//}
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(0, 0, m_view_width, m_view_height);
	
	// 2. Render scene as normal 
	glClearColor(m_bk_color[0], m_bk_color[1], m_bk_color[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the view matrix in the uniform block - we only have to do this once per loop iteration.
    const float *m = m_camera->getMatrix().get();
    glm::mat4 view =
        glm::mat4(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_shaders["BlinnPhong"]->Bind();

    Light::LinkUniform(m_shaders["BlinnPhong"], m_direct_light);

    const auto &eye_position = m_camera->getPosition();
    m_shaders["BlinnPhong"]->LinkUniformVec3("eyePosition", glm::vec3(eye_position.x, eye_position.y, eye_position.z));

    m_shaders["BlinnPhong"]->Release();

    for (auto &geometry_map : m_geometries)
    {
        geometry_map.second.Draw(m_shaders["BlinnPhong"]);
    }

    m_sky_box->Draw();
}