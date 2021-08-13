
#include "renderer_widget.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_openglwidget.h"
#include <iostream>

RendererWidget::RendererWidget(QWidget *parent, Qt::WindowFlags f)
	: QOpenGLWidget(parent, f)
	, m_pixel_ratio(devicePixelRatio())
{
	QSurfaceFormat surfaceFormat;
	surfaceFormat.setSamples(8);
	setFormat(surfaceFormat);

    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
}

RendererWidget::~RendererWidget()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_Impl_OpenglWidget_Shutdown();
	ImGui::DestroyContext();

	m_scene.reset();
}

void RendererWidget::AddGeometry(const std::string& name, const Geometry& geometry)
{
	makeCurrent();

	m_scene->AddGeometry(name, geometry);
	m_scene->SetViewCenterAndRadius(geometry.GetBoundingBoxCenter(), geometry.GetBoundingBoxRadius());
	
	update();
}

void RendererWidget::initializeGL()
{
    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return;
    }

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_Impl_OpenglWidget_Init(this);
	ImGui_ImplOpenGL3_Init();

	const GLubyte * gl_vendor = glGetString(GL_VENDOR);
	const GLubyte * gl_Renderer = glGetString(GL_RENDERER);
	const GLubyte * gl_version = glGetString(GL_VERSION);
	const GLubyte * gl_extensions = glGetString(GL_EXTENSIONS);

	printf("opengl vendor    : %s\n", gl_vendor);
	printf("opengl Renderer  : %s\n", gl_Renderer);
	printf("opengl version   : %s\n", gl_version);
	//printf("opengl extensions: %s\n", gl_extensions);

	glEnable(GL_DEPTH_TEST);
	
	m_scene.reset(new Scene());
	m_scene->InitContext();
}

void RendererWidget::paintGL()
{
	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_Impl_OpenglWidget_NewFrame();
	//ImGui::NewFrame();

	m_scene->Draw();

	//ImGui::EndFrame();
	//ImGui::Render();
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

 //   QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void RendererWidget::resizeGL(int w, int h)
{
	m_scene->SetViewPort(0, 0, w, h);
    QOpenGLWidget::resizeGL(w, h);
}

void RendererWidget::mousePressEvent(QMouseEvent *event)
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
		m_scene->SetMousePosition(x, y);
	}

	m_is_mouse_pressed = true;
    QOpenGLWidget::mousePressEvent(event);
    update();
}

void RendererWidget::mouseReleaseEvent(QMouseEvent *event)
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
		m_scene->SetMousePosition(x, y);
	}

	m_is_mouse_pressed = false;
	QOpenGLWidget::mouseReleaseEvent(event);
    update();
}

void RendererWidget::mouseMoveEvent(QMouseEvent *event)
{
	ImGuiIO &io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		// avoid mouse and key mix operate cause error
		m_is_mouse_pressed = false;
	}
	else
	{
		if (!m_is_mouse_pressed)
		{
			return;
		}

		const auto x = static_cast<float>(event->x() * m_pixel_ratio);
		const auto y = static_cast<float>(event->y() * m_pixel_ratio);

		if (event->buttons() == Qt::LeftButton)
		{
		}
		else if (event->buttons() == Qt::RightButton)
		{
			m_scene->RotateCamera(x, y);
		}
		else if (event->buttons() == Qt::MiddleButton)
		{
			m_scene->MoveCamera(x, y);
		}
	}

    QOpenGLWidget::mouseMoveEvent(event);
    update();
}

void RendererWidget::wheelEvent(QWheelEvent *event)
{
	ImGuiIO &io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		ImGui_Impl_OpenglWidget_ProcessWheelEvent(event);
	}
	else
	{
		m_scene->ZoomCamera(static_cast<float>(event->delta()));
	}

    QOpenGLWidget::wheelEvent(event);
    update();
}

void RendererWidget::keyPressEvent(QKeyEvent *event)
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
	update();
}

void RendererWidget::keyReleaseEvent(QKeyEvent *event)
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
	update();
}
