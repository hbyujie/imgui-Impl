
#include "image_widget.h"
#include "multi_images_widget.h"

#include "shader_pool.h"

MultiImagesWidget::MultiImagesWidget(QWidget *parent, Qt::WindowFlags f) : QMainWindow(parent, f)
{
    QWidget *central_widget = takeCentralWidget();
    if (central_widget)
    {
        delete central_widget;
    }

    //   for (int i = 0; i < m_max_dock; i++)
    //   {
    //       m_docks.append(new QDockWidget(QString::number(i + 1), this));
    //	m_docks[i]->hide();
    //   }

    // setDockNestingEnabled(true);

    //   addDockWidget(Qt::LeftDockWidgetArea, m_docks[0]);
    //   splitDockWidget(m_docks[0], m_docks[1], Qt::Horizontal);
    //   splitDockWidget(m_docks[1], m_docks[2], Qt::Horizontal);
    //   splitDockWidget(m_docks[0], m_docks[3], Qt::Vertical);
    //   splitDockWidget(m_docks[1], m_docks[4], Qt::Vertical);
    //   splitDockWidget(m_docks[2], m_docks[5], Qt::Vertical);
    //   splitDockWidget(m_docks[3], m_docks[6], Qt::Vertical);
    //   splitDockWidget(m_docks[4], m_docks[7], Qt::Vertical);
    //   splitDockWidget(m_docks[5], m_docks[8], Qt::Vertical);
}

MultiImagesWidget::~MultiImagesWidget()
{
}

void MultiImagesWidget::AddWidget(const int channel_id)
{
    m_image_widgets[channel_id].reset(new ImageWidget(this));
    m_image_widgets[channel_id]->SetShader(ShaderPool::GetInstance()->GetShader("ColorImage"));

    m_dock_widgets[channel_id].reset(new QDockWidget(QString("Channel: ") + QString::number(channel_id), this));
    m_dock_widgets[channel_id]->setWidget(m_image_widgets[channel_id].get());
    this->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, m_dock_widgets[channel_id].get());
}

void MultiImagesWidget::RemoveWidget(const int channel_id)
{
    m_image_widgets[channel_id]->SetShader(nullptr);
    m_image_widgets[channel_id].reset();
    m_image_widgets.erase(channel_id);

    this->removeDockWidget(m_dock_widgets[channel_id].get());
    m_dock_widgets[channel_id]->setWidget(nullptr);
    m_dock_widgets[channel_id].reset();
    m_dock_widgets.erase(channel_id);
}

void MultiImagesWidget::ReceiveImage(const int channel_id, const GLuint &texture)
{
    if (m_image_widgets.find(channel_id) != m_image_widgets.end())
    {
        m_image_widgets[channel_id]->SetImage(texture);
    }
}

void MultiImagesWidget::SetDockVis(const QList<int> &index)
{
    for (int i = 0; i < m_max_dock; i++)
    {
        m_docks[i]->hide();
    }

    for (const auto &i : index)
    {
        m_docks[i]->show();
    }
}