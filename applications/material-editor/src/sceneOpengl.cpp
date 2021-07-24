
#include "src/sceneOpengl.h"

#include "src/sceneData.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "opengl/OrbitCamera.h"
#include "opengl/gl_mesh.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_light.h"

SceneOpengl::SceneOpengl(QObject *parent) : QObject(parent)
{
    m_camera = new OrbitCamera();
}

SceneOpengl::~SceneOpengl()
{
}

void SceneOpengl::Draw()
{
    glClearColor(m_bk_color[0], m_bk_color[1], m_bk_color[2], m_bk_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &file_name : m_mesh_file_names)
    {
        const auto &mesh = SceneData::Instance()->GetMesh(file_name);
        const auto &shader = SceneData::Instance()->GetShader(mesh->GetShaderName());

        shader->Bind();
        shader->LinkUniformMat4("projection", m_projection);
        shader->LinkUniformMat4("view", m_view);
        shader->LinkUniformMat4("model", mesh->GetModelMatrix());
        shader->LinkUniformVec3("eyePosition", m_eye_pos);

		SceneData::Instance()->GetLight()->BindToShader(shader);

        for (const auto &mesh_map : mesh->GetConstPartsMeshes())
        {
            const auto &parts_mesh = mesh_map.second;

            if (parts_mesh.textures.albedo.id)
            {
                glBindTextureUnit(0, parts_mesh.textures.albedo.id);
            }
            if (parts_mesh.textures.normal.id)
            {
                glBindTextureUnit(1, parts_mesh.textures.normal.id);
            }
            if (parts_mesh.textures.metallic.id)
            {
                glBindTextureUnit(2, parts_mesh.textures.metallic.id);
            }
            if (parts_mesh.textures.roughness.id)
            {
                glBindTextureUnit(3, parts_mesh.textures.roughness.id);
            }
            if (parts_mesh.textures.ao.id)
            {
                glBindTextureUnit(4, parts_mesh.textures.ao.id);
            }

            glBindVertexArray(parts_mesh.vao);
            glDrawArrays(GL_TRIANGLES, 0, parts_mesh.vertices.size());
            glBindVertexArray(0);

			//glDrawElements()

            glBindTextureUnit(0, 0);
            glBindTextureUnit(1, 0);
            glBindTextureUnit(2, 0);
            glBindTextureUnit(3, 0);
            glBindTextureUnit(4, 0);
        }

		shader->Release();
    }

	DrawLights();
}

void SceneOpengl::AddModel(const std::string &file_name)
{
    m_mesh_file_names.insert(file_name);

    const auto &mesh = SceneData::Instance()->GetMesh(file_name);
    this->SetViewCenterAndRadius(mesh->GetBoundingBoxCenter(), mesh->GetBoundingBoxRadius());
}

void SceneOpengl::SetViewPort(GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewport(x, y, width, height);
    UpdateProjection(x, y, width, height);
}

void SceneOpengl::SetViewCenterAndRadius(const glm::vec3 &center, const float radius)
{
    Vector3 m_view_center = Vector3(center[0], center[1], center[2]);

    float scale = 3.9f;
    Vector3 eye = m_view_center + Vector3(0.f, 0.f, 1.f) * scale * radius;
    m_camera->lookAt(eye, m_view_center);
    UpdateView();
}

void SceneOpengl::SetMousePosition(const float x, const float y)
{
    m_mouse_x = x;
    m_mouse_y = y;
}

void SceneOpengl::RotateCamera(const float x, const float y)
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
    UpdateView();
}

void SceneOpengl::MoveCamera(const float x, const float y)
{
    const float MOVE_SCALE = m_camera->getDistance() * 0.001f;
    Vector2 shift_delta;
    shift_delta.x = (x - m_mouse_x) * MOVE_SCALE;
    shift_delta.y = (y - m_mouse_y) * MOVE_SCALE;
    m_mouse_x = x;
    m_mouse_y = y;

    m_camera->shift(shift_delta);
    UpdateView();
}

void SceneOpengl::ZoomCamera(const float delta)
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
    UpdateView();
}

void SceneOpengl::UpdateProjection(GLint x, GLint y, GLsizei width, GLsizei height)
{
    m_projection = glm::perspective(45.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100000.f);
}

void SceneOpengl::UpdateView()
{
    const float *m = m_camera->getMatrix().get();
    m_view =
        glm::mat4(m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);

    const Vector3 camera_pos = m_camera->getPosition();
    m_eye_pos = glm::vec3(camera_pos.x, camera_pos.y, camera_pos.z);
}

void SceneOpengl::DrawLights()
{

}