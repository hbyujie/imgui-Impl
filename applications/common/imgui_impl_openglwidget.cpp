// dear imgui: Platform Backend for QOpenGLWidget
// This needs to be used along with a Renderer (e.g. OpenGL2, OpenGL3..)

#include "imgui_impl_openglwidget.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QWheelEvent>

#include <QClipboard>
#include <QCursor>
#include <QDateTime>
#include <QGuiApplication>
#include <QPoint>

static QOpenGLWidget *g_OpenGLWidget = NULL;
static qint64 g_Time = 0;
static qint64 g_TicksPerSecond = 0;
static bool g_MouseDownStatus[ImGuiMouseButton_COUNT] = {};
static float g_MouseWheelH;
static float g_MouseWheelV;
QByteArray g_CurrentClipboardText;

const QHash<int, ImGuiKey> qt_key_map_imgui = {{Qt::Key_Tab, ImGuiKey_Tab},
                                               {Qt::Key_Left, ImGuiKey_LeftArrow},
                                               {Qt::Key_Right, ImGuiKey_RightArrow},
                                               {Qt::Key_Up, ImGuiKey_UpArrow},
                                               {Qt::Key_Down, ImGuiKey_DownArrow},
                                               {Qt::Key_PageUp, ImGuiKey_PageUp},
                                               {Qt::Key_PageDown, ImGuiKey_PageDown},
                                               {Qt::Key_Home, ImGuiKey_Home},
                                               {Qt::Key_End, ImGuiKey_End},
                                               {Qt::Key_Insert, ImGuiKey_Insert},
                                               {Qt::Key_Delete, ImGuiKey_Delete},
                                               {Qt::Key_Backspace, ImGuiKey_Backspace},
                                               {Qt::Key_Space, ImGuiKey_Space},
                                               {Qt::Key_Enter, ImGuiKey_Enter},
                                               {Qt::Key_Escape, ImGuiKey_Escape},
                                               {Qt::Key_Return, ImGuiKey_KeyPadEnter},
                                               {Qt::Key_A, ImGuiKey_A},
                                               {Qt::Key_C, ImGuiKey_C},
                                               {Qt::Key_V, ImGuiKey_V},
                                               {Qt::Key_X, ImGuiKey_X},
                                               {Qt::Key_Y, ImGuiKey_Y},
                                               {Qt::Key_Z, ImGuiKey_Z}};

const QHash<ImGuiMouseCursor, Qt::CursorShape> imgui_curosr_map_qt = {
    {ImGuiMouseCursor_Arrow, Qt::CursorShape::ArrowCursor},
    {ImGuiMouseCursor_TextInput, Qt::CursorShape::IBeamCursor},
    {ImGuiMouseCursor_ResizeAll, Qt::CursorShape::SizeAllCursor},
    {ImGuiMouseCursor_ResizeNS, Qt::CursorShape::SizeVerCursor},
    {ImGuiMouseCursor_ResizeEW, Qt::CursorShape::SizeHorCursor},
    {ImGuiMouseCursor_ResizeNESW, Qt::CursorShape::SizeBDiagCursor},
    {ImGuiMouseCursor_ResizeNWSE, Qt::CursorShape::SizeFDiagCursor},
    {ImGuiMouseCursor_Hand, Qt::CursorShape::PointingHandCursor},
    {ImGuiMouseCursor_NotAllowed, Qt::CursorShape::ForbiddenCursor},
};

bool ImGui_Impl_OpenglWidget_Init(QOpenGLWidget *window)
{
    g_OpenGLWidget = window;
    g_Time = QDateTime::currentMSecsSinceEpoch();
    g_TicksPerSecond = 1000;

    // Setup backend capabilities flags
    ImGuiIO &io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.BackendPlatformName = "imgui_impl_qopenglwidget";

    // Keyboard mapping. Dear ImGui use those indices to peek into the io.KeysDown[] array.
    for (ImGuiKey key : qt_key_map_imgui.values())
    {
        io.KeyMap[key] = key;
    }

    io.SetClipboardTextFn = [](void *user_data, const char *text) {
        Q_UNUSED(user_data);
        QGuiApplication::clipboard()->setText(text);
    };

    io.GetClipboardTextFn = [](void *user_data) {
        Q_UNUSED(user_data);
        g_CurrentClipboardText = QGuiApplication::clipboard()->text().toUtf8();
        return (const char *)g_CurrentClipboardText.data();
    };

    return true;
}

void ImGui_Impl_OpenglWidget_Shutdown()
{
    g_OpenGLWidget = NULL;
    g_Time = 0;
    g_TicksPerSecond = 0;
}

void ImGui_Impl_OpenglWidget_UpdateMousePosAndButtons()
{
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(g_OpenGLWidget != NULL);

    // Update buttons
    for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
    {
        io.MouseDown[i] = g_MouseDownStatus[i];
    }

    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by
    // user)
    if (io.WantSetMousePos)
    {
        QPoint local_pos((int)io.MousePos.x, (int)io.MousePos.y);
        const QPoint global_pos = g_OpenGLWidget->mapToGlobal(local_pos);

        QCursor cursor = g_OpenGLWidget->cursor();
        cursor.setPos(global_pos);
        g_OpenGLWidget->setCursor(cursor);
    }

    // Set mouse position
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    if (g_OpenGLWidget->isActiveWindow())
    {
        const QPoint pos = g_OpenGLWidget->mapFromGlobal(QCursor::pos());
        io.MousePos = ImVec2((float)pos.x(), (float)pos.y());
    }
}

void ImGui_Impl_OpenglWidget_UpdateMouseWheel()
{
    ImGuiIO &io = ImGui::GetIO();

    io.MouseWheelH = g_MouseWheelH;
    io.MouseWheel = g_MouseWheelV;
    g_MouseWheelH = 0;
    g_MouseWheelV = 0;
}

void ImGui_Impl_OpenglWidget_UpdateMouseCursor()
{
    ImGuiIO &io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

    const ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || (imgui_cursor == ImGuiMouseCursor_None))
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        g_OpenGLWidget->setCursor(Qt::CursorShape::BlankCursor);
    }
    else
    {
        // Translate `ImGuiMouseCursor` into `Qt::CursorShape` and show it, if we can
        if (imgui_curosr_map_qt.contains(imgui_cursor))
        {
            g_OpenGLWidget->setCursor(imgui_curosr_map_qt[imgui_cursor]);
        }
        else
        {
            g_OpenGLWidget->setCursor(Qt::CursorShape::ArrowCursor);
        }
    }
}

void ImGui_Impl_OpenglWidget_NewFrame()
{
    ImGuiIO &io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer "
                                     "backend. Missing call to renderer _NewFrame() function? e.g. "
                                     "ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    io.DisplaySize = ImVec2((float)(g_OpenGLWidget->width()), (float)(g_OpenGLWidget->height()));
    io.DisplayFramebufferScale = ImVec2(g_OpenGLWidget->devicePixelRatio(), g_OpenGLWidget->devicePixelRatio());

    // Setup time step
    qint64 current_time = QDateTime::currentMSecsSinceEpoch();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) / g_TicksPerSecond : (float)(1.0f / 60.0f);
    g_Time = current_time;

    ImGui_Impl_OpenglWidget_UpdateMousePosAndButtons();
    ImGui_Impl_OpenglWidget_UpdateMouseWheel();
    ImGui_Impl_OpenglWidget_UpdateMouseCursor();
}

void ImGui_Impl_OpenglWidget_ProcessMouseEvent(QMouseEvent *event)
{
    g_MouseDownStatus[0] = event->buttons() & Qt::LeftButton;
    g_MouseDownStatus[1] = event->buttons() & Qt::RightButton;
    g_MouseDownStatus[2] = event->buttons() & Qt::MiddleButton;
}

void ImGui_Impl_OpenglWidget_ProcessWheelEvent(QWheelEvent *event)
{
    int pixel_x = event->pixelDelta().x();
    int pixel_y = event->pixelDelta().y();
    int angle_x = event->angleDelta().x();
    int angle_y = event->angleDelta().y();

    float imgui_line_height = ImGui::GetTextLineHeight();
    float magic_number = 120.f;

    // Handle horizontal component
    if (pixel_x != 0)
    {
        g_MouseWheelH += pixel_x / imgui_line_height;
    }
    else
    {
        g_MouseWheelH += angle_x / magic_number;
    }

    // Handle vertical component
    if (pixel_y != 0)
    {
        g_MouseWheelV += pixel_y / (5 * imgui_line_height);
    }
    else
    {
        g_MouseWheelV += angle_y / magic_number;
    }
}

void ImGui_Impl_OpenglWidget_ProcessKeyEvent(QKeyEvent *event)
{
    ImGuiIO &io = ImGui::GetIO();

    const bool key_pressed = (event->type() == QEvent::KeyPress);

    // Translate `Qt::Key` into `ImGuiKey`, and apply 'pressed' state for that key
    if (qt_key_map_imgui.contains(event->key()))
    {
        io.KeysDown[qt_key_map_imgui[event->key()]] = key_pressed;
    }

    if (key_pressed)
    {
        const QString text = event->text();
        if (text.size() == 1)
        {
            io.AddInputCharacter(text.at(0).unicode());
        }
    }

#ifdef Q_OS_MAC
    io.KeyCtrl = event->modifiers() & Qt::MetaModifier;
    io.KeyShift = event->modifiers() & Qt::ShiftModifier;
    io.KeyAlt = event->modifiers() & Qt::AltModifier;
    io.KeySuper = event->modifiers() & Qt::ControlModifier; // Comamnd key
#else
    io.KeyCtrl = event->modifiers() & Qt::ControlModifier;
    io.KeyShift = event->modifiers() & Qt::ShiftModifier;
    io.KeyAlt = event->modifiers() & Qt::AltModifier;
    io.KeySuper = event->modifiers() & Qt::MetaModifier;
#endif
}