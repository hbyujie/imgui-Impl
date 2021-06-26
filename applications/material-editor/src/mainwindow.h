
#pragma once

#include <QMainWindow>
#include <memory>

class OpenglWidget;
class QToolBar;

class MainWindow : public QMainWindow
{
  public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

  private slots:
    void SlotAddModel();

  private:
    std::shared_ptr<OpenglWidget> m_opengl_widget;

    std::shared_ptr<QToolBar> m_tool_bar;
};
