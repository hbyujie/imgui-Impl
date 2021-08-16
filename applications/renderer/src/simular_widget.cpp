
#include "simular_widget.h"
#include "shader_pool.h"
#include "texture_pool.h"
#include "texture_map.h"

#include <iostream>

SimularWidget::SimularWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
{
}

SimularWidget::~SimularWidget()
{
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

	ShaderPool::GetInstance()->Initialize();
	TexturePool::GetInstance()->Update();
	TexturePool::GetInstance()->AddTexture("D:/imgui-openglwidget/data/textures/cyborg_diffuse.png");
	TexturePool::GetInstance()->AddTexture("D:/imgui-openglwidget/data/textures/mars.png");
	TexturePool::GetInstance()->AddTexture("D:/imgui-openglwidget/data/textures/back.jpg");
}

void SimularWidget::paintGL()
{
    glClearColor(0.6, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
	

	emit SendImage(0, TexturePool::GetInstance()->GetTexture("D:/imgui-openglwidget/data/textures/cyborg_diffuse.png")->GetTextureID());
	emit SendImage(1, TexturePool::GetInstance()->GetTexture("D:/imgui-openglwidget/data/textures/mars.png")->GetTextureID());
	emit SendImage(2, TexturePool::GetInstance()->GetTexture("D:/imgui-openglwidget/data/textures/back.jpg")->GetTextureID());

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void SimularWidget::resizeGL(int w, int h)
{
    makeCurrent();

    QOpenGLWidget::resizeGL(w, h);

    update();
}