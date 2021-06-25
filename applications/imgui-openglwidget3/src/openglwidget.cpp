
#include "src/openglwidget.h"

#include "backends/imgui_impl_opengl3.h"
#include "common/imgui_impl_openglwidget.h"

#include <iostream>

OpenglWidget::OpenglWidget(QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent, f)
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_Impl_OpenglWidget_Init(this);
    ImGui_ImplOpenGL3_Init();
}

void OpenglWidget::paintGL()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_Impl_OpenglWidget_NewFrame();
    ImGui::NewFrame();

    // imgui
    static bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // opengl
    glClearColor(GLclampf(0.6), GLclampf(0.6), GLclampf(0.6), GLclampf(1.0));
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void OpenglWidget::resizeGL(int w, int h)
{
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
