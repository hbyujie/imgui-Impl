
#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>

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
	void SendImage(int channel_id, GLuint texture);

  private:
};
