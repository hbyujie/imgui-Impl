
#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>

#include <memory>

class Quad;
class Shader;

class SimularWidget : public QOpenGLWidget
{
    Q_OBJECT
  public:
    explicit SimularWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~SimularWidget() override;

  private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

  signals:
    void SignalSendTextureID(const GLuint &textureID);

  private:
    GLuint m_textureID{0};

    std::shared_ptr<Shader> m_shader{nullptr};
    std::shared_ptr<Quad> m_quad{nullptr};
};
