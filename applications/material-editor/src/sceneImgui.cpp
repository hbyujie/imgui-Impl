
#include "src/sceneImgui.h"
#include "common/imgui_popup_box.h"
#include "opengl/gl_light.h"
#include "opengl/gl_mesh.h"
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

        for (auto &file_name : m_mesh_file_names)
        {
            if (ImGui::CollapsingHeader(file_name.c_str()))
            {
                ImGui::Indent();

                const auto &mesh = SceneData::Instance()->GetMesh(file_name);
                for (const auto &mesh_map : mesh->GetConstPartsMeshes())
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

    DrawLights();
}

void SceneImgui::AddModel(const std::string &file_name)
{
    m_mesh_file_names.insert(file_name);
}

void SceneImgui::DrawLights()
{
    bool is_show_light_node = true;
    if (is_show_light_node)
    {
        ImGui::Begin("LightNodes", &is_show_light_node);

        const auto &light = SceneData::Instance()->GetLight();

        auto &dir_light = light->GetVariableDirectLight();
        if (ImGui::CollapsingHeader("DirectLight"))
        {
            ImGui::Indent();

            ImGui::Checkbox("DirectLight Enabled", &dir_light.enabled);
            ImGui::InputFloat3("DirectLight Direction", &dir_light.direction[0]);
            ImGui::ColorEdit3("DirectLight Ambient", &dir_light.ambient[0]);
            ImGui::ColorEdit3("DirectLight Diffuse", &dir_light.diffuse[0]);
            ImGui::ColorEdit3("DirectLight Specular", &dir_light.specular[0]);

            ImGui::Unindent();
        }
        ImGui::Separator();

        auto *point_lights = light->GetVariablePointLights();
        for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
        {
            auto &point_light = point_lights[i];
            const std::string pre_name = "PointLight - " + std::to_string(i + 1);
            if (ImGui::CollapsingHeader(pre_name.c_str()))
            {
                ImGui::Indent();

                ImGui::Checkbox((pre_name + " Enabled").c_str(), &point_light.enabled);
                ImGui::InputFloat3((pre_name + " Position").c_str(), &point_light.position[0]);
                ImGui::ColorEdit3((pre_name + " Ambient").c_str(), &point_light.ambient[0]);
                ImGui::ColorEdit3((pre_name + " Diffuse").c_str(), &point_light.diffuse[0]);
                ImGui::ColorEdit3((pre_name + " Specular").c_str(), &point_light.specular[0]);
                ImGui::InputFloat((pre_name + " Constant").c_str(), &point_light.constant);
                ImGui::InputFloat((pre_name + " Linear").c_str(), &point_light.linear);
                ImGui::InputFloat((pre_name + " Quadratic").c_str(), &point_light.quadratic);

                ImGui::Unindent();
            }
            ImGui::Separator();
        }

		auto *spot_lights = light->GetVariableSpotLights();
		for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
		{
			auto &spot_light = spot_lights[i];
			const std::string pre_name = "SpotLight - " + std::to_string(i + 1);
			if (ImGui::CollapsingHeader(pre_name.c_str()))
			{
				ImGui::Indent();

				ImGui::Checkbox((pre_name + " Enabled").c_str(), &spot_light.enabled);
				ImGui::InputFloat3((pre_name + " Position").c_str(), &spot_light.position[0]);
				ImGui::InputFloat3((pre_name + " Direction").c_str(), &spot_light.direction[0]);
				ImGui::ColorEdit3((pre_name + " Ambient").c_str(), &spot_light.ambient[0]);
				ImGui::ColorEdit3((pre_name + " Diffuse").c_str(), &spot_light.diffuse[0]);
				ImGui::ColorEdit3((pre_name + " Specular").c_str(), &spot_light.specular[0]);
				ImGui::InputFloat((pre_name + " Constant").c_str(), &spot_light.constant);
				ImGui::InputFloat((pre_name + " Linear").c_str(), &spot_light.linear);
				ImGui::InputFloat((pre_name + " Quadratic").c_str(), &spot_light.quadratic);
				ImGui::InputFloat((pre_name + " CutOff").c_str(), &spot_light.cut_off);
				ImGui::InputFloat((pre_name + " OuterCutOff").c_str(), &spot_light.outer_cut_off);

				ImGui::Unindent();
			}
			ImGui::Separator();
		}
        ImGui::End();
    }
}