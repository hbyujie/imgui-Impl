
#include "shader_pool.h"
#include "shader.h"

#include <iostream>

ShaderPool *ShaderPool::m_instance = nullptr;
std::mutex ShaderPool::m_lock;

ShaderPool *ShaderPool::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_lock.lock();
        if (m_instance == nullptr)
        {
            m_instance = new ShaderPool();
        }
        m_lock.unlock();
    }

    return m_instance;
}

ShaderPool::ShaderPool()
{
    std::cout << "ShaderPool Construct." << std::endl;
}

ShaderPool::~ShaderPool()
{
    std::cout << "ShaderPool Deconstruct." << std::endl;
    for (auto &shader_map : m_shaders)
    {
        shader_map.second.reset();
    }
    m_shaders.clear();
}

void ShaderPool::Initialize()
{
    m_shaders["ColorImage"].reset(
        new Shader(QString(":/shaders/shader/image_map.vs"), QString(":/shaders/shader/image_color_map.fs")));

    m_shaders["DepthImage"].reset(
        new Shader(QString(":/shaders/shader/image_map.vs"), QString(":/shaders/shader/image_depth_map.fs")));

	m_shaders["NoMVP"].reset(
		new Shader(QString(":/shaders/shader/no_mvp.vs"), QString(":/shaders/shader/no_mvp.fs")));
}

const std::shared_ptr<Shader> ShaderPool::GetShader(const std::string &name) const
{
    if (m_shaders.find(name) != m_shaders.end())
    {
        return m_shaders.find(name)->second;
    }

    return nullptr;
}
