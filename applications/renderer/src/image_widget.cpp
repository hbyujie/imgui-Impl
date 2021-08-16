
#include "image_widget.h"
#include "quad.h"
#include "shader.h"

#include <iostream>

ImageWidget::ImageWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
}

ImageWidget::~ImageWidget()
{
    m_quad.reset();
}

void ImageWidget::SetImage(const GLuint &texture)
{
    m_texture = texture;
}

void ImageWidget::SetShader(const std::shared_ptr<Shader> &shader)
{
    m_shader = shader;
}

void ImageWidget::initializeGL()
{
    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return;
    }

    m_quad.reset();
    m_quad.reset(new Quad());
}

void ImageWidget::paintGL()
{
    glClearColor(0.6, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    m_quad->Draw(m_shader, m_texture);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}