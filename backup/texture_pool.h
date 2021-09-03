
#pragma once

#include <GL/glew.h>

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class TextureMap;

class TexturePool
{
  public:
    static TexturePool *GetInstance();
    ~TexturePool();

	void Update();

	void AddTexture(const std::string& image_name);

	const std::shared_ptr<TextureMap> GetTexture(const std::string& name) const;

  private:
    TexturePool();

	void DeleteAllTextures();

    static TexturePool *m_instance;
    static std::mutex m_lock;

    std::unordered_map<std::string, std::shared_ptr<TextureMap>> m_textures;
	std::vector<std::string> m_texture_names;
};
