// dear imgui: Platform Backend for QOpenGLWidget
// This needs to be used along with a Renderer (e.g. OpenGL2, OpenGL3..)

#pragma once

#include "imgui.h" // IMGUI_IMPL_API

class QOpenGLWidget;
class QMouseEvent;
class QWheelEvent;
class QKeyEvent;

IMGUI_IMPL_API bool ImGui_Impl_OpenglWidget_Init(QOpenGLWidget *window);
IMGUI_IMPL_API void ImGui_Impl_OpenglWidget_Shutdown();
IMGUI_IMPL_API void ImGui_Impl_OpenglWidget_NewFrame();

IMGUI_IMPL_API void ImGui_Impl_OpenglWidget_ProcessMouseEvent(QMouseEvent *event);
IMGUI_IMPL_API void ImGui_Impl_OpenglWidget_ProcessWheelEvent(QWheelEvent *event);
IMGUI_IMPL_API void ImGui_Impl_OpenglWidget_ProcessKeyEvent(QKeyEvent *event);