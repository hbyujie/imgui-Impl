
#pragma once

#include "imgui.h" // IMGUI_IMPL_API
#include "opengl/gl_defines.h"

#include <iostream>

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

	void StringToCharArray(const std::string &str, char *char_array)
	{
		int i = 0;
		for (i = 0; i < str.length(); ++i)
		{
			char_array[i] = str[i];
		}
		char_array[i] = '\0';
	}

	void SetTextureValue(const std::string& name, const PartsMesh &value)
	{
		mesh_name = name;
		StringToCharArray(value.textures.albedo.file_name, albedo_file);
		StringToCharArray(value.textures.normal.file_name, normal_file);
		StringToCharArray(value.textures.metallic.file_name, metallic_file);
		StringToCharArray(value.textures.roughness.file_name, roughness_file);
		StringToCharArray(value.textures.ao.file_name, ao_file);
	}

	void Show()
	{

		if (ImGui::BeginPopupModal(("Edit Texture & Material - " + mesh_name).c_str(), NULL, ImGuiWindowFlags_None))
		{
			ImGui::InputText("albedo filename", albedo_file, Max_Name_Size);
			ImGui::InputText("normal filename", normal_file, Max_Name_Size);
			ImGui::InputText("metallic filename", metallic_file, Max_Name_Size);
			ImGui::InputText("roughness filename", roughness_file, Max_Name_Size);
			ImGui::InputText("ao filename", ao_file, Max_Name_Size);

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
	std::string mesh_name;
	char albedo_file[Max_Name_Size];
	char normal_file[Max_Name_Size];
	char metallic_file[Max_Name_Size];
	char roughness_file[Max_Name_Size];
	char ao_file[Max_Name_Size];
};