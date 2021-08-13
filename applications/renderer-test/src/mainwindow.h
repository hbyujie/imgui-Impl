
#pragma once

#include <QMainWindow>
#include <memory>

class RendererWidget;
class SimularWidget;
class ImageWidget;
class QToolBar;
class QDockWidget;

class MainWindow : public QMainWindow
{
  public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

  private slots:
    void SlotObjRead();
	void SlotAssimpRead();

private:
	void InitScene();

  private:
    std::shared_ptr<RendererWidget> m_renderer_widget;
	std::shared_ptr<SimularWidget> m_simular_widget;
	std::shared_ptr<ImageWidget> m_image_widget;

	QDockWidget* m_image_dock;
	QDockWidget* m_float_dock;

    std::shared_ptr<QToolBar> m_tool_bar;
};
