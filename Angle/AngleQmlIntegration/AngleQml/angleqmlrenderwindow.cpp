#include "angleqmlrenderwindow.h"

#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
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
}

AngleQmlRenderWindow::AngleQmlRenderWindow()
    : QQuickView()
{
    _madeCurrent = false;

    connect(this, SIGNAL(beforeRendering()), SLOT(handleBeforeRendering()), Qt::DirectConnection);
}

IDirect3DSurface9* AngleQmlRenderWindow::getD3DSurfaceHandle()
{
    EGLDisplay display = eglGetCurrentDisplay();

    if(!display)
    {
        cout << "display is null." << endl;
        return NULL;
    }

    EGLBoolean result = eglSwapInterval(display, 0);
    if(!result)
        cout << "Unable to set eglSwapInterval" << endl;

    EGLSurface sfc = eglGetCurrentSurface(EGL_DRAW);
    egl::Surface* surface = static_cast<egl::Surface*>(sfc);
    rx::SwapChain *swapChain = surface->getSwapChain();
    rx::SwapChain9* swapChainD3D9 = dynamic_cast<rx::SwapChain9*>(swapChain);

    return swapChainD3D9->getRenderTarget();
}

bool AngleQmlRenderWindow::makeCurrent()
{
    if(_madeCurrent)
        return false;

    QOpenGLContext::currentContext()->makeCurrent(this);
    _madeCurrent = true;
    return true;
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
