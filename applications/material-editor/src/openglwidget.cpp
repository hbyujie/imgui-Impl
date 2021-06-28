
#include "src/openglwidget.h"

#include "backends/imgui_impl_opengl3.h"
#include "common/imgui_impl_openglwidget.h"

#include "src/sceneData.h"
#include "src/sceneImgui.h"
#include "src/sceneOpengl.h"

#include <iostream>

OpenglWidget::OpenglWidget(QWidget *parent, Qt::WindowFlags f)
	: QOpenGLWidget(parent, f)
	, m_pixel_ratio(devicePixelRatio())
{
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
}

OpenglWidget::~OpenglWidget()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_Impl_OpenglWidget_Shutdown();
    ImGui::DestroyContext();
}

void OpenglWidget::initializeGL()
{
    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return;
    }

	const GLubyte * gl_vendor = glGetString(GL_VENDOR);
	const GLubyte * gl_renderer = glGetString(GL_RENDERER);
	const GLubyte * gl_version = glGetString(GL_VERSION);
	const GLubyte * gl_extensions = glGetString(GL_EXTENSIONS);

	printf("opengl vendor    £º%s\n", gl_vendor);
	printf("opengl renderer  £º%s\n", gl_renderer);
	printf("opengl version   £º%s\n", gl_version);
	//printf("opengl extensions£º%s\n", gl_extensions);

	glEnable(GL_DEPTH_TEST);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_Impl_OpenglWidget_Init(this);
    ImGui_ImplOpenGL3_Init();

	if (SceneData::Instance() != nullptr)
	{
		;
	}
    m_scene_imgui = std::make_shared<SceneImgui>();
    m_scene_opengl = std::make_shared<SceneOpengl>();
}

void OpenglWidget::paintGL()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_Impl_OpenglWidget_NewFrame();
    ImGui::NewFrame();

    // imgui
    m_scene_imgui->Draw();

    // opengl
    m_scene_opengl->Draw();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void OpenglWidget::resizeGL(int w, int h)
{
	m_scene_opengl->SetViewPort(0, 0, w, h);
    QOpenGLWidget::resizeGL(w, h);
}

void OpenglWidget::mousePressEvent(QMouseEvent *event)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        ImGui_Impl_OpenglWidget_ProcessMouseEvent(event);
    }
    else
    {
		const auto x = static_cast<float>(event->x() * m_pixel_ratio);
		const auto y = static_cast<float>(event->y() * m_pixel_ratio);
		m_scene_opengl->SetMousePosition(x, y);
    }

    QOpenGLWidget::mousePressEvent(event);

    update();
}

void OpenglWidget::mouseReleaseEvent(QMouseEvent *event)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        ImGui_Impl_OpenglWidget_ProcessMouseEvent(event);
    }
    else
    {
		const auto x = static_cast<float>(event->x() * m_pixel_ratio);
		const auto y = static_cast<float>(event->y() * m_pixel_ratio);
		m_scene_opengl->SetMousePosition(x, y);
    }

    QOpenGLWidget::mouseReleaseEvent(event);

    update();
}

void OpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
    }
    else
    {
		const auto x = static_cast<float>(event->x() * m_pixel_ratio);
		const auto y = static_cast<float>(event->y() * m_pixel_ratio);

		if (event->buttons() == Qt::LeftButton)
		{
		}
		else if (event->buttons() == Qt::RightButton)
		{
			m_scene_opengl->RotateCamera(x, y);
		}
		else if (event->buttons() == Qt::MiddleButton)
		{
			m_scene_opengl->MoveCamera(x, y);
		}
    }

    QOpenGLWidget::mouseMoveEvent(event);

    update();
}

void OpenglWidget::wheelEvent(QWheelEvent *event)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        ImGui_Impl_OpenglWidget_ProcessWheelEvent(event);
    }
    else
    {
		m_scene_opengl->ZoomCamera(static_cast<float>(event->delta()));
    }

    QOpenGLWidget::wheelEvent(event);

    update();
}

void OpenglWidget::keyPressEvent(QKeyEvent *event)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        ImGui_Impl_OpenglWidget_ProcessKeyEvent(event);
    }
    else
    {
    }

    QOpenGLWidget::keyPressEvent(event);
}

void OpenglWidget::keyReleaseEvent(QKeyEvent *event)
{
    ImGuiIO &io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        ImGui_Impl_OpenglWidget_ProcessKeyEvent(event);
    }
    else
    {
    }

    QOpenGLWidget::keyPressEvent(event);
}

void OpenglWidget::AddModel(const QString &file_name)
{
    makeCurrent();

    SceneData::Instance()->AddModel(file_name.toLocal8Bit().toStdString());
	
	m_scene_imgui->AddModel(file_name.toLocal8Bit().toStdString());

	m_scene_opengl->AddModel(file_name.toLocal8Bit().toStdString());

    update();
}
