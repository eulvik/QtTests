#ifndef RENDERCONTROLNATIVEINTERFACE_H
#define RENDERCONTROLNATIVEINTERFACE_H

#include "angleqml_global.h"

#include <windows.h>

class RenderControlWindow;
class QGuiApplication;

class ANGLEQMLSHARED_EXPORT RenderControlNativeInterface
{
public:
    RenderControlNativeInterface();

    virtual bool initialize(HWND hwnd, int width, int height);
    virtual void renderFrame();
    virtual void *getBackBufferPointer();
    virtual void resizeSurface(int width, int height);
    virtual void shutdown();
	virtual void animate();

private:
    RenderControlWindow *_controlWindow;
    bool _initialized;
    int _width, _height;
    QGuiApplication *_application;
};

#endif // RENDERCONTROLNATIVEINTERFACE_H
