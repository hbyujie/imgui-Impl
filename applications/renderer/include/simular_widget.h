
#pragma once

#include <GL/glew.h>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <memory>

#include "light.h"

struct Primitive;
class SimularScene;
class Shader;
class Camera;
class ViewController;
class SkyBox;

class SimularWidget : public QOpenGLWidget
{
    Q_OBJECT
  public:
    explicit SimularWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SimularWidget() override;

    void SetScenePtr(const std::shared_ptr<SimularScene> ptr)
    {
        m_scene_ptr = ptr;
    }

    const std::shared_ptr<SimularScene> GetScenePtr()
    {
        return m_scene_ptr;
    }

  private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

  signals:
    void SendImage(int channel_id, GLuint texture);

  private:
    int m_pixel_ratio{1};
	GLclampf m_bk_color[4]{ 178 / 255.f, 159 / 255.f, 250 / 255.f, 1.0f };

    std::shared_ptr<SimularScene> m_scene_ptr{nullptr};
    std::shared_ptr<Shader> m_lighting_shader{nullptr};
    std::shared_ptr<Shader> m_depth_map_shader{nullptr};
    std::shared_ptr<Shader> m_sky_box_shader{nullptr};

    std::shared_ptr<Camera> m_camera{nullptr};
    std::shared_ptr<ViewController> m_view_controller{nullptr};

	DirectLight m_direct_light;

    std::shared_ptr<SkyBox> m_sky_box{nullptr};

    GLuint m_ubo_matrices{0};
};
