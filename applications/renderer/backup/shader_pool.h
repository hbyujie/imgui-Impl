
#pragma once

#include <GL/glew.h>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class Shader;

class ShaderPool
{
  public:
    static ShaderPool *GetInstance();
    ~ShaderPool();

    void Initialize();

    const std::shared_ptr<Shader> GetShader(const std::string &name) const;

  private:
    ShaderPool();

    static ShaderPool *m_instance;
    static std::mutex m_lock;

    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
};
