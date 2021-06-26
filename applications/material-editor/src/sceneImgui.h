
#pragma once

#include <QObject>
#include <imgui.h>

#include <memory>

class TextureEdit;

class SceneImgui : public QObject
{
  public:
    SceneImgui(QObject *parent = nullptr);
    ~SceneImgui() override;

    void Draw();

  private:
    std::shared_ptr<TextureEdit> m_imgui_texture_edit;
};
