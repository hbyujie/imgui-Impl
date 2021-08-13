
#include "image_widget.h"
#include "quad.h"
#include "shader.h"
#include "texture_manage.h"

#include <iostream>

ImageWidget::ImageWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
}

ImageWidget::~ImageWidget()
{
	TextureManage::DeleteTexture(m_textureID);
}

void ImageWidget::SlotReceiveTexture(const GLuint &textureID)
{
	m_textureID = textureID;
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

	if (m_shader == nullptr)
	{
		m_shader.reset(new Shader(QString(":/shaders/shader/image_map_1.vs"), QString(":/shaders/shader/image_map_1.fs")));
	}

	if (m_quad == nullptr)
	{
		m_quad.reset(new Quad());
	}
}

void ImageWidget::paintGL()
{
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	m_quad->Draw(m_shader, m_textureID);

	QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void ImageWidget::resizeGL(int w, int h)
{
	makeCurrent();

	QOpenGLWidget::resizeGL(w, h);

	update();
}