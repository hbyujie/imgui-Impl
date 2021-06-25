
#include "src/openglwidget.h"
#include <QApplication>

int main(int argc, char **argv)
{
    const QApplication app(argc, argv);

    OpenglWidget widget;
	widget.setWindowTitle("Imgui-OpenglWidget3");
	widget.resize(1280, 720);
	widget.show();

    return QApplication::exec();
}
