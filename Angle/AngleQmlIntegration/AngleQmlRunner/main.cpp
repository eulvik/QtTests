#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QUrl>
#include <QQmlContext>
#include <QTimer>

#include <angleqmlrenderwindow.h>
#include <rendercontrolwindow.h>

#include <iostream>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QTimer timer;
    timer.setInterval(16);
    RenderControlWindow window;
    window.resize(1024, 768);
    window.show();

    timer.connect(&timer, SIGNAL(timeout()), &window, SLOT(updateQuick()));
	timer.start();
    return app.exec();
}

