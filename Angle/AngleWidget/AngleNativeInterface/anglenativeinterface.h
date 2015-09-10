#ifndef ANGLENATIVEINTERFACE_H
#define ANGLENATIVEINTERFACE_H

#include "anglenativeinterface_global.h"

#include <Windows.h>
#include <string>
#include "MouseEvent.h"
#include <QtCore/QElapsedTimer>
#include <QOpenGLContext>

class QGuiApplication;
class RenderSurface;
class AngleGLScene;

class ANGLENATIVEINTERFACESHARED_EXPORT AngleNativeInterface
{

public:
    AngleNativeInterface();
    virtual bool initializeAngle(HWND hwnd, int width, int height);
    virtual void renderFrame();
    virtual void *getBackBufferPointer();
    virtual void resizeRenderSurface(int width, int height);
    virtual void setWireframeRenderMode(bool);
    virtual bool getWireframeRenderMode() const;
    virtual void shutdown();
    virtual void animate();

    virtual bool mouseMoved(const MouseEvent& mouseEvent);
    virtual bool mouseWheel(const MouseEvent& mouseEvent);
    virtual bool mouseDown(const MouseEvent& mouseEvent);
    virtual bool mouseUp(const MouseEvent& mouseEvent);

private:

    QGuiApplication* _app;
    RenderSurface* _surf;
    AngleGLScene* _glScene;
    QOpenGLContext *_context;
    QElapsedTimer _fr_timer;
    int _num_frames;

    int _width, _height;

    void* _back_buffer_ptr;

    bool _shutdown;
    bool _wireframe;
    bool _gl_initialized;

    bool initializeGL();
    bool createGraphicsContext();
};

#endif // ANGLENATIVEINTERFACE_H
