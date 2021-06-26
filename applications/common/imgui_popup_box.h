
#pragma once

#include "imgui.h" // IMGUI_IMPL_API

struct MeshTreeNode
{
    std::string name;

    std::string albedo;
    std::string normal;
    std::string metallic;
    std::string roughness;
    std::string ao;
};

const int Max_Name_Size = 256;
class TextureEdit
{
  public:
    TextureEdit()
    {
    }
    ~TextureEdit()
    {
    }
    void SetValue(const MeshTreeNode &value)
    {
        int i = 0;
        for (i = 0; i < value.albedo.length(); ++i)
        {
            albedo_file[i] = value.albedo[i];
        }
        albedo_file[i] = '\0';
    }
    void Show()
    {

        if (ImGui::BeginPopupModal("TextureEdit", NULL, ImGuiWindowFlags_None))
        {
            ImGui::Text("Mesh Name:");
            ImGui::SameLine();
            // ImGui::Text(value.name.c_str());
            ImGui::Separator();

            ImGui::Text("albedo   : ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::InputText("##", albedo_file, Max_Name_Size);

            if (ImGui::Button("OK", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

  private:
    MeshTreeNode m_mesh_tree_node;
    char albedo_file[Max_Name_Size];
};
