
#include "src/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
#if (QT_VERSION > QT_VERSION_CHECK(5, 4, 0))
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
#endif
    const QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.setWindowTitle("Demo");
    mainwindow.resize(800, 600);
    // mainwindow.showMaximized();
    mainwindow.show();

    return QApplication::exec();
}