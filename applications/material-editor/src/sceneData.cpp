
#include "src/sceneData.h"
#include "opengl/model_reader.h"
#include "opengl/gl_upload.h"
#include "opengl/gl_shader.h"
#include "opengl/gl_texture_manager.h"

SceneData *SceneData::m_instance = nullptr;

SceneData *SceneData::Instance()
{
    if (m_instance == nullptr)
    {
        m_instance = new SceneData;
    }
    return m_instance;
}

void SceneData::Delete()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = nullptr;
    }
}

SceneData::SceneData(QObject *parent) : QObject(parent)
	, m_data_dir("D:/imgui-openglwidget/data/")
{
	m_shaders["BlinnPhong"] = std::make_shared<GLShader>((m_data_dir + "shader/blinnPhong.vs"), (m_data_dir + "shader/blinnPhong.fs"));

	m_texture_manager = std::make_shared<GLTextureManager>();
}

SceneData::~SceneData()
{
}

void SceneData::AddModel(const std::string &file_name)
{
    if (m_meshes.find(file_name) != m_meshes.end())
    {
        return;
    }

    Mesh &mesh = m_meshes[file_name];
    ModelReader reader(file_name, &mesh);

	mesh.shader_name = "BlinnPhong";

	for (auto& part_mesh : mesh.parts_meshes)
	{
		GLUpload::UploadVertex(part_mesh.second);

		m_texture_manager->UploadTextures(part_mesh.second.textures);
	}

}