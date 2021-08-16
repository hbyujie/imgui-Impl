
#include "texture_pool.h"
#include "texture_map.h"

#include <iostream>

TexturePool *TexturePool::m_instance = nullptr;
std::mutex TexturePool::m_lock;

TexturePool *TexturePool::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_lock.lock();
        if (m_instance == nullptr)
        {
            m_instance = new TexturePool();
        }
        m_lock.unlock();
    }

    return m_instance;
}

TexturePool::TexturePool()
{
    std::cout << "TexturePool Construct." << std::endl;
}

TexturePool::~TexturePool()
{
    std::cout << "TexturePool Deconstruct." << std::endl;
    DeleteAllTextures();
}

void TexturePool::Update()
{
    DeleteAllTextures();

    for (const auto &image_name : m_texture_names)
    {
        m_textures[image_name].reset(new TextureMap(image_name));
    }
}

void TexturePool::AddTexture(const std::string &image_name)
{
    m_textures[image_name].reset(new TextureMap(image_name));
    m_texture_names.push_back(image_name);
}

const std::shared_ptr<TextureMap> TexturePool::GetTexture(const std::string &name) const
{
    if (m_textures.find(name) != m_textures.end())
    {
        return m_textures.find(name)->second;
    }

    return nullptr;
}

void TexturePool::DeleteAllTextures()
{
    for (auto &texture_map : m_textures)
    {
        texture_map.second.reset();
    }
    m_textures.clear();
}