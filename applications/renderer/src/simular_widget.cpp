
#include "simular_widget.h"
//#include "shader_pool.h"
//#include "texture_map.h"
//#include "texture_pool.h"

#include "shader.h"
#include "simular_scene.h"

SimularWidget::SimularWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
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

    m_scene_ptr.reset(new SimularScene());
    m_lighting_shader.reset(new Shader(QString(":/shaders/shader/no_mvp.vs"), QString(":/shaders/shader/no_mvp.fs")));
    // m_lighting_shader.reset(new Shader(QString(":/shaders/shader/blinnPhong.vs"),
    // QString(":/shaders/shader/blinnPhong.fs")));

    m_depth_map_shader;

    // TexturePool::GetInstance()->Update();
    // TexturePool::GetInstance()->AddTexture("D:/imgui-openglwidget/data/textures/cyborg_diffuse.png");
    // TexturePool::GetInstance()->AddTexture("D:/imgui-openglwidget/data/textures/mars.png");
    // TexturePool::GetInstance()->AddTexture("D:/imgui-openglwidget/data/textures/back.jpg");

    glGenBuffers(1, &m_ubo_matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ubo_matrices, 0, 2 * sizeof(glm::mat4));

    // ShaderPool::GetInstance()->Initialize();
    // ShaderPool::GetInstance()->GetShader("NoMVP")->LinkUniformBlock("Matrices", 0);
}

void SimularWidget::paintGL()
{
    glClearColor(0.6, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // emit SendImage(0, TexturePool::GetInstance()
    //                      ->GetTexture("D:/imgui-openglwidget/data/textures/cyborg_diffuse.png")
    //                      ->GetTextureID());
    // emit SendImage(
    //    1, TexturePool::GetInstance()->GetTexture("D:/imgui-openglwidget/data/textures/mars.png")->GetTextureID());
    // emit SendImage(
    //    2, TexturePool::GetInstance()->GetTexture("D:/imgui-openglwidget/data/textures/back.jpg")->GetTextureID());

    glViewport(0, 0, this->width(), this->height());

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    m_scene_ptr->Draw(m_lighting_shader);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void SimularWidget::resizeGL(int w, int h)
{
    const auto &width_f = static_cast<float>(this->width());
    const auto &height_f = static_cast<float>(this->width());
    glm::mat4 projection = glm::perspective(m_fov, width_f / height_f, m_near_plane, m_far_plane);

    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo_matrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    QOpenGLWidget::resizeGL(w, h);
}