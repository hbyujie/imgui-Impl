
#include "src/sceneImgui.h"
#include "common/imgui_popup_box.h"
#include "src/sceneData.h"

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

    bool show_mesh_node_tree = true;
    if (show_mesh_node_tree)
    {
        ImGui::Begin("ModelNodes", &show_mesh_node_tree);

        for (auto &mesh_file_name : m_mesh_file_names)
        {
            if (ImGui::CollapsingHeader(mesh_file_name.c_str()))
            {
                ImGui::Indent();

                const auto &mesh = SceneData::Instance()->GetMesh(mesh_file_name);
                for (auto &mesh_map : mesh.parts_meshes)
                {
                    if (ImGui::Button(mesh_map.first.c_str()))
                    {
                        m_imgui_texture_edit->SetTextureValue(mesh_map.first, mesh_map.second);
                        ImGui::OpenPopup(("Edit Texture & Material - " + mesh_map.first).c_str());
                    }

                    ImGui::SameLine();
                    ImGui::Text("Edit Texture & Material");
                }
                m_imgui_texture_edit->Show();

                ImGui::Unindent();
                ImGui::Separator();
            }
        }

        ImGui::End();
    }
}

void SceneImgui::AddModel(const std::string &file_name)
{
    m_mesh_file_names.insert(file_name);
}