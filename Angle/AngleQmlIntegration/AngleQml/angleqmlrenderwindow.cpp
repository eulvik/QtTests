#include "angleqmlrenderwindow.h"

#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QQmlEngine>

#include <d3d9.h>
#include <dxgi.h>
#include <libEGL/Surface.h>
#if QT_VERSION >= 0x050400
    #include <libGLESv2/renderer/d3d/d3d9/SwapChain9.h>
#elif QT_VERSION >= 0x050300
    #include <libGLESv2/renderer/d3d9/SwapChain9.h>
#else
    #include <libGLESv2/renderer/SwapChain9.h>
#endif
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <iostream>
using namespace std;

static EGLint const attribute_list[] = {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_NONE
};

AngleQmlRenderWindow::AngleQmlRenderWindow(QQmlEngine *engine, QWindow *parent)
    : QQuickView(engine, parent), _rotation(0)
{
    cout << "Creating AngleQmlRenderWindow." << endl;
    _madeCurrent = false;
    connect(this, SIGNAL(beforeRendering()), SLOT(handleBeforeRendering()), Qt::DirectConnection);
}

AngleQmlRenderWindow::AngleQmlRenderWindow()
    : QQuickView()
{

    _madeCurrent = false;
    connect(this, SIGNAL(beforeRendering()), SLOT(handleBeforeRendering()), Qt::DirectConnection);
}

IDirect3DSurface9* AngleQmlRenderWindow::getD3DSurfaceHandle()
{
    cout << "getD3DSurfaceHandle called" << endl;

    EGLDisplay display = eglGetCurrentDisplay();

    if(!display)
    {
        cout << "display is null." << endl;
        return NULL;
    }
    else
        cout << "Found actual display" << endl;

    EGLBoolean result = eglSwapInterval(display, 0);
    if(!result)
        cout << "Unable to set eglSwapInterval" << endl;
    else
        cout << "Managed to swap" << endl;

    EGLSurface sfc = eglGetCurrentSurface(EGL_DRAW);
    cout << "sfc: " << sfc << endl;
    egl::Surface* surface = static_cast<egl::Surface*>(sfc);
    cout << "casted surface " << surface << endl;
    rx::SwapChain *swapChain = surface->getSwapChain();
    cout << "swap chain: " << swapChain << endl;
    rx::SwapChain9* swapChainD3D9 = dynamic_cast<rx::SwapChain9*>(swapChain);
    cout << "swapChainD3D9 " << swapChainD3D9 << endl;
    return swapChainD3D9->getRenderTarget();
}

bool AngleQmlRenderWindow::makeCurrent()
{
    if(_madeCurrent)
        return false;

    cout << "makeCurrent context: " << QOpenGLContext::currentContext() << endl;
    QOpenGLContext::currentContext()->makeCurrent(this);
    _madeCurrent = true;
    return true;
}

void AngleQmlRenderWindow::renderOne()
{
    cout << "renderOne()" << endl;
    update();
}

void AngleQmlRenderWindow::handleBeforeRendering()
{
    makeCurrent();
    getD3DSurfaceHandle();
    _rotation += 1;
    if(_rotation > 360)
        _rotation = 0;

    rotationChanged();
}
