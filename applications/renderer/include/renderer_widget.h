
#pragma once

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <memory>
#include <unordered_map>
#include "scene.h"
#include "geometry.h"


class RendererWidget : public QOpenGLWidget
{
public:
	explicit RendererWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	~RendererWidget() override;

	void AddGeometry(const std::string& name, const Geometry& geometry);

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
	
private:
	int m_pixel_ratio;
	std::unique_ptr<Scene> m_scene{ nullptr };

	bool m_is_mouse_pressed{ false };
};
