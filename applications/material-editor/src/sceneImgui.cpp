
#include "src/sceneImgui.h"
#include "common/imgui_popup_box.h"

SceneImgui::SceneImgui(QObject *parent) : QObject(parent)
{
    m_imgui_texture_edit = std::make_shared<TextureEdit>();
}

SceneImgui::~SceneImgui()
{
}

void SceneImgui::Draw()
{
    static bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    static MeshTreeNode test_value;

    if (ImGui::Button("TextureEditBtn1"))
    {
        test_value.name = "sfasdgx1";
        test_value.albedo = "2213123adfdc";

        m_imgui_texture_edit->SetValue(test_value);
        ImGui::OpenPopup("TextureEdit");
    }

    if (ImGui::Button("TextureEditBtn2"))
    {
        test_value.name = "2213123adfdc";
        test_value.albedo = "2213123adfdc";

        m_imgui_texture_edit->SetValue(test_value);
        ImGui::OpenPopup("TextureEdit");
    }

    m_imgui_texture_edit->Show();
}
