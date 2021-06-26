
#include <QApplication>
#include <QFileDialog>
#include <QToolBar>

#include "src/mainwindow.h"
#include "src/openglwidget.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    m_tool_bar = std::make_shared<QToolBar>();
    this->addToolBar(m_tool_bar.get());

    QAction *add_model = m_tool_bar->addAction(QString("Add Model"));
    connect(add_model, &QAction::triggered, this, &MainWindow::SlotAddModel);

    m_opengl_widget = std::make_shared<OpenglWidget>();
    this->setCentralWidget(m_opengl_widget.get());
}

MainWindow::~MainWindow()
{
}

void MainWindow::SlotAddModel()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Add Model", ".", "*.obj");
    if (!file_name.isEmpty())
    {
        m_opengl_widget->AddModel(file_name);
    }
}
