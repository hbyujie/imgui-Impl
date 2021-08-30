
#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <memory>

struct Primitive;
class SimularScene;
class Shader;

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

  signals:
    void SendImage(int channel_id, GLuint texture);

  private:
    std::shared_ptr<SimularScene> m_scene_ptr{nullptr};
    std::shared_ptr<Shader> m_lighting_shader{nullptr};
    std::shared_ptr<Shader> m_depth_map_shader{nullptr};

    float m_fov{45.f};
    float m_near_plane{0.1f};
    float m_far_plane{100000.f};

    GLuint m_ubo_matrices{0};
};
