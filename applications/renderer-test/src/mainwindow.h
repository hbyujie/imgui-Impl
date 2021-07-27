
#pragma once

#include <QMainWindow>
#include <memory>

class RendererWidget;
class QToolBar;

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

    std::shared_ptr<QToolBar> m_tool_bar;
};
