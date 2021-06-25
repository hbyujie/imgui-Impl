
#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>

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
};
