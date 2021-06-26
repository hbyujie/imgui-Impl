
#pragma once

#include <QObject>

class SceneData : public QObject
{
  public:
    static SceneData *Instance();

    void Delete();

    ~SceneData() override;

  private:
    SceneData(QObject *parent = nullptr);

    static SceneData *m_instance;
};
