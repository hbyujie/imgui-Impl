
#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <memory>

class TextureEdit;
class SceneImgui;
class SceneOpengl;

class OpenglWidget : public QOpenGLWidget
{
  public:
    explicit OpenglWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~OpenglWidget() override;

  private:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

  public:
    void AddModel(const QString &file_name);

  private:
	  int m_pixel_ratio;
	  std::shared_ptr<SceneImgui> m_scene_imgui;
	  std::shared_ptr<SceneOpengl> m_scene_opengl;
};
