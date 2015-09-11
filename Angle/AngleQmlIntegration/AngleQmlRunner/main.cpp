#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QUrl>
#include <QQmlContext>

#include <angleqmlrenderwindow.h>
#include <rendercontrolwindow.h>

#include <iostream>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    RenderControlWindow window;
    window.resize(1024, 768);
    window.show();
    return app.exec();
}

