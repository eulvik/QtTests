#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#include "angleglwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    app.setApplicationName("cube");
    app.setApplicationVersion("0.1");
    AngleGLWidget widget;
    widget.show();
    return app.exec();
}
