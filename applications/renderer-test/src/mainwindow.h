
#pragma once

#include <QMainWindow>
#include <memory>

class RendererWidget;
class SimularWidget;
class MultiImagesWidget;
class ImageWidget;
class QToolBar;
class QDockWidget;

class MainWindow : public QMainWindow
{
  public:
    explicit MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~MainWindow();

  private slots:
    void SlotTestDrawElements();
    void SlotObjRead();
    void SlotAssimpRead();

  private:
    std::shared_ptr<SimularWidget> m_simular_widget;
    std::shared_ptr<MultiImagesWidget> m_multi_images_widget;
    std::shared_ptr<QDockWidget> m_multi_images_dock_widget;

    std::shared_ptr<QToolBar> m_tool_bar;
};
