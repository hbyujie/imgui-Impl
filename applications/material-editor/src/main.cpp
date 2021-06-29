
#include "src/mainwindow.h"
#include <QApplication>

int main(int argc, char **argv)
{
    const QApplication app(argc, argv);

    MainWindow mainwindow;
    mainwindow.setWindowTitle("Material Editor");
    mainwindow.resize(1280, 720);
    mainwindow.show();

    return QApplication::exec();
}
