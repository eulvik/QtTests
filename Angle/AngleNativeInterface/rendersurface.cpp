#include "rendersurface.h"

#include <QVariant>

#include <d3d9.h>
#include <dxgi.h>
#include <hash_map>
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
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : warning X0000: "

RenderSurface::RenderSurface(QWindow* parent)
    : QWindow(parent), _context(NULL)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

bool RenderSurface::createGraphicsContext()
{
    if (!isExposed())
        return false;

    _context = new QOpenGLContext(this);
    _context->setFormat(requestedFormat());
    _context->create();

    _context->makeCurrent(this);

    initializeOpenGLFunctions();

    setVisible(false);

    return true;
}

IDirect3DSurface9* RenderSurface::getD3DSurfaceHandle()
{
    if(_context == NULL)
    {
        if(!createGraphicsContext())
            return NULL;
    }

    EGLDisplay dpy = eglGetCurrentDisplay();
    if(!dpy)
        std::cout << "display is null" << std::endl;
    EGLBoolean result = eglSwapInterval(dpy, 0);

    EGLSurface sfc = eglGetCurrentSurface(EGL_DRAW);
    egl::Surface* surface = static_cast<egl::Surface*>(sfc);
    rx::SwapChain9* swap_chain9 = dynamic_cast<rx::SwapChain9*>(surface->getSwapChain());

    return swap_chain9->getRenderTarget();
}

bool RenderSurface::makeCurrent()
{
    if (!_context)
        return false;
    return _context->makeCurrent(this);
}
