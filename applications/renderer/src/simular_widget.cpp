
#include "simular_widget.h"

#include "camera.h"
#include "shader.h"
#include "simular_scene.h"
#include "sky_box.h"
#include "view_controllers/xy_orbit_view_controller.h"

SimularWidget::SimularWidget(QWidget *parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f), m_pixel_ratio(devicePixelRatio())
{
}

SimularWidget::~SimularWidget()
{
    glDeleteBuffers(1, &m_ubo_matrices);
}

void SimularWidget::initializeGL()
{
    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        printf("Failed to initialize GLEW.\n");
        return;
    }

    glEnable(GL_DEPTH_TEST);

    m_scene_ptr.reset(new SimularScene());
    m_lighting_shader.reset(new Shader(QString(":/shaders/shader/no_mvp.vs"), QString(":/shaders/shader/no_mvp.fs")));
    // m_lighting_shader.reset(
    //    new Shader(QString(":/shaders/shader/blinnPhong.vs"), QString(":/shaders/shader/blinnPhong.fs")));
    m_depth_map_shader;
    m_sky_box_shader.reset(new Shader(QString(":/shaders/shader/skybox.vs"), QString(":/shaders/shader/skybox.fs")));

    m_camera.reset(new Camera());
    m_camera->LookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    m_view_controller.reset(new XYOrbitViewController());
    m_view_controller->SetCamera(m_camera);

    m_sky_box.reset(new SkyBox());
    m_sky_box->SetTextures({
        QString("D:/imgui-openglwidget/data/textures/skybox/right.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/left.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/top.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/bottom.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/front.jpg"),
        QString("D:/imgui-openglwidget/data/textures/skybox/back.jpg"),
    });

    glGenBuffers(1, &m_ubo_matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo_matrices, 0, 2 * sizeof(glm::mat4));
}

void SimularWidget::paintGL()
{
	glClearColor(m_bk_color[0], m_bk_color[1], m_bk_color[2], m_bk_color[3]);
	glClear(GL_COLOR_BUFFER_BIT);

    // emit SendImage(0, TexturePool::GetInstance()
    //                      ->GetTexture("D:/imgui-openglwidget/data/textures/cyborg_diffuse.png")
    //                      ->GetTextureID());
    // emit SendImage(
    //    1, TexturePool::GetInstance()->GetTexture("D:/imgui-openglwidget/data/textures/mars.png")->GetTextureID());
    // emit SendImage(
    //    2, TexturePool::GetInstance()->GetTexture("D:/imgui-openglwidget/data/textures/back.jpg")->GetTextureID());

    glViewport(0, 0, this->width(), this->height());

    m_camera->SetViewPort(0, 0, this->width(), this->height());
    glm::mat4 projection = m_camera->GetProjection();
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glm::mat4 view = m_camera->GetView();
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_scene_ptr->Draw(m_lighting_shader);

    m_sky_box->Draw(m_sky_box_shader);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void SimularWidget::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void SimularWidget::mousePressEvent(QMouseEvent *event)
{
    const auto x = static_cast<float>(event->x() * m_pixel_ratio);
    const auto y = static_cast<float>(event->y() * m_pixel_ratio);
    m_view_controller->SetMousePosition(x, y);

    QOpenGLWidget::mousePressEvent(event);
    update();
}

void SimularWidget::mouseReleaseEvent(QMouseEvent *event)
{
    const auto x = static_cast<float>(event->x() * m_pixel_ratio);
    const auto y = static_cast<float>(event->y() * m_pixel_ratio);
    m_view_controller->SetMousePosition(x, y);

    QOpenGLWidget::mouseReleaseEvent(event);
    update();
}

void SimularWidget::mouseMoveEvent(QMouseEvent *event)
{
    const auto x = static_cast<float>(event->x() * m_pixel_ratio);
    const auto y = static_cast<float>(event->y() * m_pixel_ratio);

    if (event->buttons() == Qt::LeftButton)
    {
    }
    else if (event->buttons() == Qt::RightButton)
    {
        m_view_controller->RotateCamera(x, y);
    }
    else if (event->buttons() == Qt::MiddleButton)
    {
        m_view_controller->MoveCamera(x, y);
    }

    QOpenGLWidget::mouseMoveEvent(event);
    update();
}

void SimularWidget::wheelEvent(QWheelEvent *event)
{
    m_view_controller->ZoomCamera(static_cast<float>(event->delta()));

    QOpenGLWidget::wheelEvent(event);
    update();
}
