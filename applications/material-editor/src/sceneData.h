
#pragma once

#include "opengl/gl_defines.h"
#include <QObject>
#include <string>
#include <unordered_map>
#include <memory>

class GLShader;
class GLTextureManager;

class SceneData : public QObject
{
  public:
    static SceneData *Instance();

    void Delete();

    ~SceneData() override;

    void AddModel(const std::string &file_name);

	const Mesh& GetMesh(const std::string &file_name) { return m_meshes[file_name]; }

	const std::shared_ptr<GLShader>& GetShader(const std::string &file_name) { return m_shaders[file_name]; }

  private:
    SceneData(QObject *parent = nullptr);

    static SceneData *m_instance;
	std::string m_data_dir;

    std::unordered_map<std::string, Mesh> m_meshes;

	std::unordered_map<std::string, std::shared_ptr<GLShader> > m_shaders;

	std::shared_ptr<GLTextureManager> m_texture_manager;
};
