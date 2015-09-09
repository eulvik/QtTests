#include "angleqmlnativeinterface.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "angleqmlrendersurface.h"

#include <iostream>

using namespace std;

AngleQmlNativeInterface::AngleQmlNativeInterface() :
    _renderSurface(NULL),
    _shutdown(false),
    _backBufferPointer(NULL),
    _glInitialized(false)
{
    char** argv = new char*[1];
    argv[0] = "dummyArg";
    int argc = 1;
    if(QCoreApplication::startingUp())
    {
        _application = new QGuiApplication (argc, argv);
        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    }
}

bool AngleQmlNativeInterface::initializeAngle(HWND hwnd, int width, int height)
{
    cout << "HWND:" << hwnd << " width=" << width << " height=" << height << endl;

    // Initialize rendering surface
    QWindow* syswin = QWindow::fromWinId((WId)hwnd);
    _renderSurface = new AngleQmlRenderSurface(NULL, syswin);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24); // Needed for OpenGL ES
    format.setAlphaBufferSize(8);

    _renderSurface->setFormat(format);
    _renderSurface->resize(1, 1);
    _renderSurface->show();

    _width = width;
    _height = height;

    return true;
}

bool AngleQmlNativeInterface::initializeGL()
{
    QOpenGLContext::currentContext()->makeCurrent(_renderSurface);
    auto gl = QOpenGLContext::currentContext()->functions();

    gl->glEnable(GL_DEPTH_TEST);
    gl->glEnable(GL_CULL_FACE);

    _glInitialized = true;

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
    cout << "getBackBufferPointer() called." << endl;
    if(!_renderSurface)
        return NULL;

    // We operate on a 1x1 surface until we have finished
    // initializing GL, so do not perform resize until done.
    if(_glInitialized)
    {
        if(_width != _renderSurface->width() || _height != _renderSurface->height())
        {
            _backBufferPointer = NULL;
            _renderSurface->resize(_width, _height);
        }
    }

    if(_backBufferPointer == NULL)
        _backBufferPointer = _renderSurface->getD3DSurfaceHandle();

    return _backBufferPointer;
}

void AngleQmlNativeInterface::renderFrame()
{
}

void AngleQmlNativeInterface::shutdown()
{

}
