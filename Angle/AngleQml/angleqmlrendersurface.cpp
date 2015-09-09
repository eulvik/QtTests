#include "angleqmlrendersurface.h"

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

static EGLint const attribute_list[] = {
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_NONE
};

AngleQmlRenderSurface::AngleQmlRenderSurface(QQmlEngine *engine, QWindow *parent)
    : QQuickView(engine, parent)
{
}

AngleQmlRenderSurface::AngleQmlRenderSurface()
    : QQuickView()
{
    _madeCurrent = false;

    connect(this, SIGNAL(beforeRendering()), SLOT(handleBeforeRendering()), Qt::DirectConnection);
}

IDirect3DSurface9* AngleQmlRenderSurface::getD3DSurfaceHandle()
{
    std::cout << "Checking error before." << std::endl;
    checkError();
    EGLDisplay display = eglGetCurrentDisplay();
    std::cout << "Checking error after." << std::endl;
    checkError();

    if(!display)
    {
        std::cout << "display is null." << std::endl;
        return NULL;
    }

    EGLBoolean result = eglSwapInterval(display, 0);
    if(!result)
        std::cout << "Unable to set eglSwapInterval" << std::endl;

    EGLSurface sfc = eglGetCurrentSurface(EGL_DRAW);
    egl::Surface* surface = static_cast<egl::Surface*>(sfc);
    rx::SwapChain *swapChain = surface->getSwapChain();
    rx::SwapChain9* swapChainD3D9 = dynamic_cast<rx::SwapChain9*>(swapChain);

    return swapChainD3D9->getRenderTarget();
}

bool AngleQmlRenderSurface::makeCurrent()
{
    if(_madeCurrent)
        return false;

    QOpenGLContext::currentContext()->makeCurrent(this);
    _madeCurrent = true;
    return true;
}

void AngleQmlRenderSurface::handleBeforeRendering()
{
    makeCurrent();
    QOpenGLContext *context = QOpenGLContext::currentContext();
    std::cout << "BeforeRendering. GLContext is GLES: " << context->isOpenGLES() << " isValid: " << context->isValid() << std::endl;
    getD3DSurfaceHandle();
}

void AngleQmlRenderSurface::checkError()
{
    return;
    QOpenGLFunctions* gl = QOpenGLContext::currentContext()->functions();
    GLenum err = gl->glGetError();
    if(err != GL_NO_ERROR)
    {
        std::string gl_error = "GL_NO_ERROR";
        switch(err)
        {
        case GL_INVALID_ENUM:
            gl_error = "GL_INVALID_ENUM";
            break;

        case GL_INVALID_VALUE:
            gl_error = "GL_INVALID_VALUE";
            break;

        case GL_INVALID_OPERATION:
            gl_error = "GL_INVALID_OPERATION";
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            gl_error = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;

        case GL_OUT_OF_MEMORY:
            gl_error = "GL_OUT_OF_MEMORY";
            break;

        default:
            std::stringstream ss(gl_error);
            ss << "Unknown error: code = " << (int)err;
            break;
        }
        std::cout << "Found error: " << gl_error << std::endl;
    }
    std::cout << "No error found." << std::endl;
}
