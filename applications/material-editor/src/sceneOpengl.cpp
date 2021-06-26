
#include "src/sceneOpengl.h"

SceneOpengl::SceneOpengl(QObject *parent) : QObject(parent)
{
}

SceneOpengl::~SceneOpengl()
{
}

void SceneOpengl::Draw()
{
    glClearColor(m_bk_color[0], m_bk_color[1], m_bk_color[2], m_bk_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}
