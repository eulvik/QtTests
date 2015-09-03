#include "qmlrendersurface.h"

QmlRenderSurface::QmlRenderSurface(const QUrl &source, QWindow *parent)
    : QQuickView(source, parent)
{    
}

IDirect3DSurface9 * QmlRenderSurface::getD3DSurfaceHandle()
{
    return NULL;
//    if(_context == NULL)
//    {
//        if(!createGraphicsContext())
//            return NULL;
//    }

//#ifdef QT_OPENGL_ES_2_ANGLE
//    EGLDisplay dpy = eglGetCurrentDisplay();
//    EGLBoolean result = eglSwapInterval(dpy, 0);

//    EGLSurface sfc = eglGetCurrentSurface(EGL_DRAW);
//    egl::Surface* surface = static_cast<egl::Surface*>(sfc);
//    rx::SwapChain9* swap_chain9 = dynamic_cast<rx::SwapChain9*>(surface->getSwapChain());

//    return swap_chain9->getRenderTarget();
//#else
//    #pragma message(__LOC__ "The M3D Globe Native -> Managed interface needs to be compiled with ANGLE OpenGL libraries")
//    return NULL;
//#endif
}

bool QmlRenderSurface::makeCurrent()
{
    return false;
}
