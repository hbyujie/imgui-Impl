
#pragma once

#include <QObject>
#include <memory>
#include <string>
#include <unordered_map>

class GLShader;
class GLMesh;
class GLLight;

class SceneData : public QObject
{
  public:
    static SceneData *Instance();

    void Delete();

    ~SceneData() override;

    void AddModel(const std::string &file_name);

    const std::shared_ptr<GLMesh> GetMesh(const std::string &file_name)
    {
        return m_meshes[file_name];
    }

    const std::shared_ptr<GLShader> GetShader(const std::string &file_name)
    {
        return m_shaders[file_name];
    }

	const std::shared_ptr<GLLight> GetLight() { return m_light; }

  private:
    SceneData(QObject *parent = nullptr);

    static SceneData *m_instance;
    std::string m_data_dir;

    std::unordered_map<std::string, std::shared_ptr<GLMesh>> m_meshes;

    std::unordered_map<std::string, std::shared_ptr<GLShader>> m_shaders;

	std::shared_ptr<GLLight> m_light;
};
