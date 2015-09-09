#include <QApplication>
#include "angleglwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("cube");
    a.setApplicationVersion("0.1");
    AngleGLScene widget;
    widget.show();

    return a.exec();
}
