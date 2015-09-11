#include "angleqmlnativeinterface.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlEngine>
#include <QQmlContext>

#include "angleqmlrenderwindow.h"

#include <iostream>

using namespace std;

AngleQmlNativeInterface::AngleQmlNativeInterface() :
    _renderSurface(NULL),
    _shutdown(false),
    _backBufferPointer(NULL),
    _glInitialized(false)
{
    cout << "Creating Native Interface." << endl;
}

bool AngleQmlNativeInterface::initializeAngle(HWND hwnd, int width, int height)
{
    char** argv = new char*[1];
    argv[0] = "dummyArg";
    int argc = 1;
    if(QCoreApplication::startingUp())
    {
        _application = new QGuiApplication (argc, argv);
        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    }

    cout << "HWND:" << hwnd << " width=" << width << " height=" << height << endl;

    // Initialize rendering surface
    QWindow* syswin = QWindow::fromWinId((WId)hwnd);
    QQmlEngine *engine = new QQmlEngine();
    _renderSurface = new AngleQmlRenderWindow(engine, syswin);

    cout << "Render Surface created." << endl;
    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24); // Needed for OpenGL ES
    format.setAlphaBufferSize(8);

    _renderSurface->setFormat(format);
    _renderSurface->resize(width, height);
    _renderSurface->show();

    _width = width;
    _height = height;

    initializeGL();
    cout << "Done initializing Angle." << endl;

    return true;
}

bool AngleQmlNativeInterface::initializeGL()
{
    cout << "Initializing GL" << endl;
    QOpenGLContext::currentContext()->makeCurrent(_renderSurface);
    auto gl = QOpenGLContext::currentContext()->functions();

    gl->glEnable(GL_DEPTH_TEST);
    gl->glEnable(GL_CULL_FACE);

    _glInitialized = true;

    cout << "Done initializing GL" << endl;

    return true;
}

void AngleQmlNativeInterface::resizeRenderSurface(int width, int height)
{
    cout << "Resizing render window, width=" << width << " height=" << height << endl;
    _width = width;
    _height = height;
    _renderSurface->resize(_width, _height);
}

void* AngleQmlNativeInterface::getBackBufferPointer()
{
    cout << "getBackBufferPointer() called. _renderSurface" << _renderSurface << " glInitialized: " << _glInitialized << endl;
    if(!_renderSurface)
        return NULL;

    if(_glInitialized)
    {
        if(_width != _renderSurface->width() || _height != _renderSurface->height())
        {
            _backBufferPointer = NULL;
            _renderSurface->resize(_width, _height);
        }
    }

    cout << "Doing actual back buffer poiner call." << endl;
    if(_backBufferPointer == NULL)
        _backBufferPointer = _renderSurface->getD3DSurfaceHandle();

    return _backBufferPointer;
}

void AngleQmlNativeInterface::renderFrame()
{
    _renderSurface->renderOne();
}

void AngleQmlNativeInterface::shutdown()
{

}
