#include "rendersurface.h"

#include <QVariant>

#ifdef QT_OPENGL_ES_2
#ifdef QT_OPENGL_ES_2_ANGLE
#include <d3d9.h>
#include <hash_map>
#include <libEGL/Surface.h>
#if QT_VERSION >= 0x050300
#include <libGLESv2/renderer/d3d9/SwapChain9.h>
#else
#include <libGLESv2/renderer/SwapChain9.h>
#endif
#endif
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#include <iostream>
using namespace std;

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : warning X0000: "

RenderSurface::RenderSurface(QWindow* parent, QOpenGLContext *context)
    : QWindow(parent), QOpenGLFunctions(context), _context(context)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

//bool RenderSurface::createGraphicsContext()
//{
//    cout << "createGraphicsContext, isExposed() = " << isExposed() << endl;
//    if (!isExposed())
//        return false;

//    _context = new QOpenGLContext(this);
//    cout << "Context newed " << _context << endl;
//    _context->setFormat(requestedFormat());
//    _context->create();
//    cout << "Context created" << endl;
//    _context->makeCurrent(this);
//    cout << "Context made current" << endl;

//    initializeOpenGLFunctions();


//    cout << "Done initializing openGL functions" << endl;
//    setVisible(false);
//    cout << "Done creating graphicsContext" << endl;
//    return true;
//}

IDirect3DSurface9* RenderSurface::getD3DSurfaceHandle()
{
    if(_context == NULL)
    {
        if(!createGraphicsContext())
            return NULL;
    }

#ifdef QT_OPENGL_ES_2_ANGLE
    cout << "About to get actual surface" << endl;
    EGLDisplay dpy = eglGetCurrentDisplay();
    EGLBoolean result = eglSwapInterval(dpy, 0);

    EGLSurface sfc = eglGetCurrentSurface(EGL_DRAW);
    egl::Surface* surface = static_cast<egl::Surface*>(sfc);
    rx::SwapChain9* swap_chain9 = dynamic_cast<rx::SwapChain9*>(surface->getSwapChain());

    return swap_chain9->getRenderTarget();
#else
    #pragma message(__LOC__ "The M3D Globe Native -> Managed interface needs to be compiled with ANGLE OpenGL libraries")
    return NULL;
#endif
}

bool RenderSurface::makeCurrent()
{
    if (!_context)
        return false;
    return _context->makeCurrent(this);
}
