
#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>

#include <memory>

class Quad;
class Shader;

class ImageWidget : public QOpenGLWidget
{
    Q_OBJECT
  public:
    explicit ImageWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ImageWidget() override;

  public slots:
    void SlotReceiveTexture(const GLuint &textureID);

  private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

  private:
    GLuint m_textureID{0};

    std::shared_ptr<Shader> m_shader{nullptr};
    std::shared_ptr<Quad> m_quad{nullptr};
};
