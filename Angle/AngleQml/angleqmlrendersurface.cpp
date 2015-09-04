#include "angleqmlrendersurface.h"

#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QtPlatformHeaders/QEGLNativeContext>
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

    connect(this, SIGNAL(openglContextCreated(QOpenGLContext*)), SLOT(handleOpenGLContextCreated(QOpenGLContext*)));
    connect(this, SIGNAL(beforeRendering()), SLOT(handleBeforeRendering()));
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


    //QOpenGLContext::currentContext()->makeCurrent(this);
    createNewGLContext();
    _madeCurrent = true;
    return true;
}

void AngleQmlRenderSurface::handleOpenGLContextCreated(QOpenGLContext *context)
{
    std::cout << "OpenGL context created: " << context << std::endl;
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

void AngleQmlRenderSurface::createNewGLContext()
{
    QOpenGLContext::currentContext()->makeCurrent(0);
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    NativeWindowType native_window;
    EGLint num_config;
    /* get an EGL display connection */
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    /* initialize the EGL display connection */
    eglInitialize(display, NULL, NULL);
    /* get an appropriate EGL frame buffer configuration */
    eglChooseConfig(display, attribute_list, &config, 1, &num_config);
//    /* create an EGL rendering context */
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
//    /* create a native window */
//    /* create an EGL window surface */
    surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)this->winId(), NULL);
//    /* connect the context to the surface */
    //eglMakeCurrent(display, surface, surface, context);
//    /* clear the color buffer */
    QEGLNativeContext ctx(context, display);
    QOpenGLContext::currentContext()->setNativeHandle(QVariant::fromValue<QEGLNativeContext>(ctx));
    bool success = QOpenGLContext::currentContext()->create();
    QOpenGLContext::currentContext()->makeCurrent(this);
}
