#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QUrl>
#include <QQmlContext>

#include <angleqmlrenderwindow.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QGuiApplication application(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

    //QQmlApplicationEngine engine;
    AngleQmlRenderWindow surf;//
    surf.rootContext()->setContextProperty("vm", &surf);
    //QSurfaceFormat f = surf->format();
    //f.setRenderableType(QSurfaceFormat::OpenGLES);
    //f.setVersion(2, 0);
    //f.setSwapInterval(0);
    //surf->setFormat(f);

    surf.setSource(QUrl("qrc:/main.qml"));
    surf.setWidth(300);
    surf.setHeight(300);
    surf.show();


    std::cout << "status " << surf.status() << std::endl;
    return application.exec();
}

