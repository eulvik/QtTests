#include "anglenativeinterface.h"
#include "angleglscene.h"
#include "rendersurface.h"

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <QGuiApplication>
#include <QMouseEvent>
#include <QWindow>

using namespace std;

static char* dummyArg = "dummyArg";

static Qt::MouseButtons getMouseButtons(const MouseButtonStates& ms)
{
    Qt::MouseButtons buttons = 0x0;
    if(ms.isLeftButtonPressed()) buttons |= Qt::LeftButton;
    if(ms.isRightButtonPressed()) buttons |= Qt::RightButton;
    if(ms.isMiddleButtonPressed()) buttons |= Qt::MiddleButton;

    return buttons;
}

static Qt::MouseButton getMouseButton(const MouseButtonStates& ms)
{
    Qt::MouseButton button = Qt::NoButton;
    switch(ms.getButton())
    {
        case MouseButtonStates::LeftButton:
        button = Qt::LeftButton;
        break;
        case MouseButtonStates::MiddleButton:
        button = Qt::MiddleButton;
        break;
        case MouseButtonStates::RightButton:
        button = Qt::RightButton;
        break;
    }

    return button;
}

static QMouseEvent toQMouseEvent(QEvent::Type type, const MouseEvent& mouseEvent)
{
    Qt::MouseButtons buttons = getMouseButtons(mouseEvent.getButtonStates());
    Qt::MouseButton button = getMouseButton(mouseEvent.getButtonStates());
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;

    return QMouseEvent(type, QPointF(mouseEvent.getX(), mouseEvent.getY()), button, buttons, modifiers);
}

static QWheelEvent toQWheelEvent(const MouseEvent& mouseEvent)
{
    Qt::MouseButtons buttons = getMouseButtons(mouseEvent.getButtonStates());
    Qt::KeyboardModifiers modifiers = Qt::NoModifier;

    return QWheelEvent(QPointF(mouseEvent.getX(), mouseEvent.getY()),
                       (int)mouseEvent.getDeltaZ(),
                       buttons, modifiers);
}


AngleNativeInterface::AngleNativeInterface() :
    _surf(NULL),
    _num_frames(0),
    _shutdown(false),
    _back_buffer_ptr(NULL),
    _gl_initialized(false)
{
    char** argv = new char*[1];
    argv[0] = dummyArg;
    int argc = 1;
    if(QCoreApplication::startingUp())
        _app = new QGuiApplication (argc, argv);
    _glScene = new AngleGLScene();
}

bool AngleNativeInterface::initializeAngle(HWND hwnd, int width, int height)
{
    cout << "HWND:" << hwnd << " width=" << width << " height=" << height << endl;

    // Initialize rendering surface
    QWindow* syswin = QWindow::fromWinId((WId)hwnd);
    _surf = new RenderSurface(syswin);

    QSurfaceFormat format;
    format.setSamples(16);
    format.setDepthBufferSize(24); // Needed for OpenGL ES
    format.setAlphaBufferSize(8);

    _surf->setFormat(format);
    _surf->resize(1, 1);
    _surf->show();

    _width = width;
    _height = height;

    return true;
}

//===========================================================================

bool AngleNativeInterface::initializeGL()
{
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);

   _glScene->initializeGL(QGLContext::fromOpenGLContext(_surf->glContext()));
   _fr_timer.start();
   _gl_initialized = true;

   return true;
}

//===========================================================================

void AngleNativeInterface::resizeRenderSurface(int width, int height)
{
    cout << "Resizing render window, width=" << width << " height=" << height << endl;
    _width = width;
    _height = height;
    _glScene->resizeGL(_width, _height);
}

//===========================================================================

void* AngleNativeInterface::getBackBufferPointer()
{
    if(!_surf)
        return NULL;

    // We operate on a 1x1 surface until we have finished
    // initializing GL, so do not perform resize until done.
    if(_gl_initialized)
    {
        if(_width != _surf->width() || _height != _surf->height())
        {
            _back_buffer_ptr = NULL;
            _surf->resize(_width, _height);
        }
    }

    if(_back_buffer_ptr == NULL)
        _back_buffer_ptr = _surf->getD3DSurfaceHandle();

    return _back_buffer_ptr;
}

//===========================================================================

void AngleNativeInterface::renderFrame()
{
    if (_shutdown)
        return;

    if(_back_buffer_ptr == NULL)
    {
        if(!getBackBufferPointer())
            return;
    }

    if(!_surf->makeCurrent())
        return;

    if(!_gl_initialized)
    {
        if(!initializeGL())
            return;
    }

    glClearColor(0.3f, 0.3f, 0.35f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifndef QT_OPENGL_ES // Wireframe rendering not supported under ES.
    if(_wireframe)
    {
        glPushAttrib(GL_POLYGON_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
#endif

    if(_glScene)
        _glScene->render();

#ifndef QT_OPENGL_ES // Wireframe rendering not supported under ES.
    if(_wireframe)
        glPopAttrib();
#endif

    _num_frames++;

    // Sample frame rate each second
    qint64 elapsed_ms = _fr_timer.elapsed();
    if(elapsed_ms > 1000)
    {
       double fps = 1000.0*_num_frames/elapsed_ms;
       cout << "fps = " << fps << endl;
       _fr_timer.restart();
       _num_frames = 0;
    }

    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        string gl_error = "GL_NO_ERROR";
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
            stringstream ss(gl_error);
            ss << "Unknown error: code = " << (int)err;
            break;
        }

        cerr << "GL Error: " << gl_error << endl;
    }
}

void AngleNativeInterface::setWireframeRenderMode(bool wireFrameMode)
{
    _wireframe = wireFrameMode;

#ifdef QT_OPENGL_ES
    //_globe->setWireframe(_wireframe);
#endif
}

bool AngleNativeInterface::getWireframeRenderMode() const
{
    return _wireframe;
}


void AngleNativeInterface::shutdown()
{
    _shutdown = true;
    _glScene = NULL;
}

void AngleNativeInterface::animate()
{
    _glScene->animate();
}

bool AngleNativeInterface::mouseMoved(const MouseEvent& mouseEvent)
{
    if(_glScene)
    {
        QMouseEvent e = toQMouseEvent(QEvent::MouseMove, mouseEvent);
        return _glScene->mouseMoveEvent(&e);
    }
    return false;
}

bool AngleNativeInterface::mouseWheel(const MouseEvent& mouseEvent)
{
    if(_glScene)
    {
        QWheelEvent e = toQWheelEvent(mouseEvent);
        return _glScene->mouseWheelEvent(&e);
    }
    return false;
}

bool AngleNativeInterface::mouseDown(const MouseEvent& mouseEvent)
{
    if(_glScene)
    {
        QMouseEvent e = toQMouseEvent(QEvent::MouseButtonPress, mouseEvent);
        return _glScene->mousePressEvent(&e);
    }

    cout << endl;
    return false;
}

bool AngleNativeInterface::mouseUp(const MouseEvent& mouseEvent)
{
    if(_glScene)
    {
        QMouseEvent e = toQMouseEvent(QEvent::MouseButtonRelease, mouseEvent);
        return _glScene->mouseReleaseEvent(&e);
    }
    return false;
}
