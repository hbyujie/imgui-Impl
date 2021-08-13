
#include "simular_widget.h"
#include "quad.h"
#include "shader.h"
#include "texture_manage.h"

#include <iostream>

SimularWidget::SimularWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
}

SimularWidget::~SimularWidget()
{
	TextureManage::DeleteTexture(m_textureID);
}

void SimularWidget::initializeGL()
{
    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return;
    }

    if (m_shader == nullptr)
    {
        m_shader.reset(new Shader(QString(":/shaders/shader/image_map_330.vs"), QString(":/shaders/shader/image_map_330.fs")));
    }

	if (m_quad == nullptr)
	{
		m_quad.reset(new Quad());
	}

	m_textureID = TextureManage::LoadTexture("D:/imgui-openglwidget/data/textures/cyborg_diffuse.png");

}

void SimularWidget::paintGL()
{
    glClearColor(0.6, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

	m_quad->Draw(m_shader, m_textureID);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void SimularWidget::resizeGL(int w, int h)
{
    makeCurrent();

    QOpenGLWidget::resizeGL(w, h);

	emit SignalSendTextureID(m_textureID);

    update();
}