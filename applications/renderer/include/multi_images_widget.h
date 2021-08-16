
#pragma once

#include <QDockWidget>
#include <QList>
#include <QMainWindow>

#include <memory>
#include <string>
#include <unordered_map>

class ImageWidget;
class Shader;

class MultiImagesWidget : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MultiImagesWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~MultiImagesWidget() override;

    void AddWidget(const int channel_id, const std::shared_ptr<Shader> &shader);

    void RemoveWidget(const int channel_id);

  public slots:
    void ReceiveImage(const int channel_id, const GLuint &texture);

  private:
    std::unordered_map<int, std::shared_ptr<QDockWidget>> m_dock_widgets;
    std::unordered_map<int, std::shared_ptr<ImageWidget>> m_image_widgets;
};
