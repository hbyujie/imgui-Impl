
#pragma once

#include <GL/glew.h>
#include <QObject>

class SceneOpengl : public QObject
{
  public:
    SceneOpengl(QObject *parent = nullptr);
    ~SceneOpengl() override;

    void Draw();

  private:
    GLclampf m_bk_color[4]{178 / 255.f, 159 / 255.f, 250 / 255.f, 1.0f};
};
