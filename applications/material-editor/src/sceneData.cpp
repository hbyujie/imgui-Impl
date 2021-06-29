
#include "src/sceneData.h"
#include "opengl/gl_mesh.h"
#include "opengl/gl_shader.h"
#include "opengl/model_reader.h"

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

SceneData::SceneData(QObject *parent) : QObject(parent), m_data_dir("D:/imgui-openglwidget/data/")
{
    m_shaders["BlinnPhong"] =
        std::make_shared<GLShader>((m_data_dir + "shader/blinnPhong.vs"), (m_data_dir + "shader/blinnPhong.fs"));
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

    m_meshes[file_name] = std::make_shared<GLMesh>();
    ModelReader reader(file_name, m_meshes[file_name]);

    m_meshes[file_name]->SetShaderName("BlinnPhong");
    m_meshes[file_name]->Upload();
}