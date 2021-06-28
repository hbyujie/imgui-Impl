
#pragma once

#include <QObject>
#include <imgui.h>
#include <memory>
#include <unordered_set>

class TextureEdit;

class SceneImgui : public QObject
{
  public:
    SceneImgui(QObject *parent = nullptr);
    ~SceneImgui() override;

    void Draw();

    void AddModel(const std::string &file_name);

  private:
    std::unordered_set<std::string> m_mesh_file_names;
    std::shared_ptr<TextureEdit> m_imgui_texture_edit;
};
