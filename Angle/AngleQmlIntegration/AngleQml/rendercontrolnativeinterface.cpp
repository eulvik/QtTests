#include "rendercontrolnativeinterface.h"

#include "rendercontrolwindow.h"

#include <QGuiApplication>

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

RenderControlNativeInterface::RenderControlNativeInterface() :
    _initialized(false)
{
    char** argv = new char*[1];
    argv[0] = "dummyArg";
    int argc = 1;
    if(QCoreApplication::startingUp())
    {
        _application = new QGuiApplication (argc, argv);
        QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    }
}

bool RenderControlNativeInterface::initialize(int width, int height)
{
    if(_initialized)
        return true;

	cout << "Creating RendeControlWindow." << endl;
    _controlWindow = new RenderControlWindow();
	cout << "Resizing surface." << endl;
    resizeSurface(width, height);
    _initialized = true;

	cout << "Showing window" << endl;
	_controlWindow->show();
	cout << "Done initializing" << endl;
    return true;
}

void RenderControlNativeInterface::renderFrame()
{
	bool ready = _controlWindow->isReady();
	cout << "RenderControlNativeInterface::renderFrame(). Ready: " << ready << endl;
    if(!_controlWindow || !ready)
        return;

    _controlWindow->updateQuick();
}

void *RenderControlNativeInterface::getBackBufferPointer()
{
    //cout << "getBackBufferPointer() called. _controlWindow" << _controlWindow <<  " initialized: " << _initialized << endl;
    if(!_controlWindow || !_controlWindow->isReady())
        return nullptr;

    if(_initialized)
    {
		cout << "_width=" << _width << " width=" << _controlWindow->width() << " _height=" << _height << " height=" << _controlWindow->height() << endl;
        if(_width != _controlWindow->width() || _height != _controlWindow->height())
        {
            resizeSurface(_width, _height);
        }
    }

    
	if (!_controlWindow->makeCurrent())
		return nullptr;

	cout << "eglGetCurrentDisplay();" << endl;
    EGLDisplay display = eglGetCurrentDisplay();

    if(!display)
    {
        cout << "display is null." << endl;
        return nullptr;
    }
        
	cout << "Found actual display" << endl;

    EGLBoolean result = eglSwapInterval(display, 0);
    
	if(!result)
        cout << "Unable to set eglSwapInterval" << endl;
    else
        cout << "Managed to swap" << endl;

    EGLSurface sfc = eglGetCurrentSurface(EGL_DRAW);
    cout << "sfc: " << sfc << endl;
    egl::Surface* surface = static_cast<egl::Surface*>(sfc);
    cout << "casted surface " << surface << endl;
    rx::SwapChain *swapChain = surface->getSwapChain();
    cout << "swap chain: " << swapChain << endl;
    rx::SwapChain9* swapChainD3D9 = dynamic_cast<rx::SwapChain9*>(swapChain);
    cout << "swapChainD3D9 " << swapChainD3D9 << endl;
    return swapChainD3D9->getRenderTarget();
}

void RenderControlNativeInterface::resizeSurface(int width, int height)
{
    _width = width;
    _height = height;

    _controlWindow->resize(width, height);
}

void RenderControlNativeInterface::shutdown()
{
    _controlWindow->close();
    delete _application;
}

