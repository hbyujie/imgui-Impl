
#include "src/sceneData.h"

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

SceneData::SceneData(QObject *parent) : QObject(parent)
{
}

SceneData::~SceneData()
{
}
