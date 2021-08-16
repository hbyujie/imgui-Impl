
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

    void SetImage(const GLuint &texture);

    void SetShader(const std::shared_ptr<Shader> &shader);

  private:
    void initializeGL() override;
    void paintGL() override;

  private:
    GLuint m_texture{0};
    std::shared_ptr<Quad> m_quad{nullptr};
    std::shared_ptr<Shader> m_shader{nullptr};
};
