#ifndef ANGLEQMLNATIVEINTERFACE_H
#define ANGLEQMLNATIVEINTERFACE_H

#include "angleqml_global.h"
#include <windows.h>

class QGuiApplication;
class AngleQmlRenderWindow;

class ANGLEQMLSHARED_EXPORT AngleQmlNativeInterface
{
public:
    AngleQmlNativeInterface();

    virtual bool initializeAngle(HWND hwnd, int width, int height);
    virtual void renderFrame();
    virtual void *getBackBufferPointer();
    virtual void resizeRenderSurface(int width, int height);
    virtual void shutdown();

private:
    QGuiApplication *_application;
    AngleQmlRenderWindow *_renderSurface;

    int _width, _height;

    void* _backBufferPointer;

    bool _shutdown;
    bool _wireframe;
    bool _glInitialized;

    bool initializeGL();
    bool createRenderSurface();
};

#endif // ANGLEQMLNATIVEINTERFACE_H
