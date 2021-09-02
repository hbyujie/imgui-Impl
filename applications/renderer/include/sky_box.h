#pragma once

#include <GL/glew.h>
#include <memory>
#include <vector>
#include <QString>
//#include "texture_manage.h"

class Shader;

class SkyBox
{
  public:
	 SkyBox();
    ~SkyBox();

    void SetName(const std::string &name);

	void SetTextures(const std::vector<QString>& cube_texture);

    void SetTextures(const std::vector<std::string>& cube_texture);

    void SetShader(const std::shared_ptr<Shader> &shader);

    void Draw();
	
  private:
    std::string m_name;
	
    // primitive buffer
	GLuint m_vao{ 0 };
	GLuint m_vbo{ 0 };

	GLuint m_cube_texture{ 0 };
	
    std::shared_ptr<Shader> m_current_shader{nullptr};
};