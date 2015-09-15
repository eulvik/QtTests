#include "rendercontrolwindow.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOffscreenSurface>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <iostream>

using namespace std;

RenderControlWindow::RenderControlWindow()
    : _rootItem(nullptr),
      _fbo(nullptr),
      _quickInitialized(false),
      _quickReady(false)
{
	cout << "Cunstructor RenderControlWindow." << endl;
    setSurfaceType(QSurface::OpenGLSurface);

    QSurfaceFormat format;
    // Qt Quick may need a depth and stencil buffer. Always make sure these are available.
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);

	cout << "Setting format." << endl;
    setFormat(format);

	cout << "Constructing OpenGLContext" << endl;
    _openGLContext = new QOpenGLContext;

	cout << "Setting OpenGLContext format." << endl;
    _openGLContext->setFormat(format);

	cout << "_openGLContext->create()" << endl;
    _openGLContext->create();

	cout << "Constructing QOffscreenSurface." << endl;
    _offscreenSurface = new QOffscreenSurface;
    // Pass m_context->format(), not format. Format does not specify and color buffer
    // sizes, while the context, that has just been created, reports a format that has
    // these values filled in. Pass this to the offscreen surface to make sure it will be
    // compatible with the context's configuration.
    _offscreenSurface->setFormat(_openGLContext->format());

	cout << "_offscreenSurface->create()" << endl;
    _offscreenSurface->create();

	cout << "Constructing QQuickRenderControl." << endl;
    _renderControl = new QQuickRenderControl(this);

    // Create a QQuickWindow that is associated with out render control. Note that this
    // window never gets created or shown, meaning that it will never get an underlying
    // native (platform) window.
	cout << "Constructing QQuickWindow" << endl;
    _quickWindow = new QQuickWindow(_renderControl);

	cout << "Constructing QQmlEngine" << endl;
    // Create a QML engine.
    _qmlEngine = new QQmlEngine;
    if (!_qmlEngine->incubationController())
        _qmlEngine->setIncubationController(_quickWindow->incubationController());

    // Now hook up the signals. For simplicy we don't differentiate between
    // renderRequested (only render is needed, no sync) and sceneChanged (polish and sync
    // is needed too).
	cout << "Connecting signals/slots" << endl;
    connect(_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &RenderControlWindow::createFbo);
    connect(_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &RenderControlWindow::destroyFbo);
    //connect(_renderControl, &QQuickRenderControl::renderRequested, this, &RenderControlWindow::updateQuick);
    //connect(_renderControl, &QQuickRenderControl::sceneChanged, this, &RenderControlWindow::updateQuick);
}

RenderControlWindow::~RenderControlWindow()
{
    // Make sure the context is current while doing cleanup. Note that we use the
    // offscreen surface here because passing 'this' at this point is not safe: the
    // underlying platform window may already be destroyed. To avoid all the trouble, use
    // another surface that is valid for sure.
    _openGLContext->makeCurrent(_offscreenSurface);

    // Delete the render control first since it will free the scenegraph resources.
    // Destroy the QQuickWindow only afterwards.
    delete _renderControl;

    delete _qmlComponent;
    delete _quickWindow;
    delete _qmlEngine;
    delete _fbo;

    _openGLContext->doneCurrent();

    delete _offscreenSurface;
    delete _openGLContext;
}

void RenderControlWindow::createFbo()
{
	cout << "RenderControlWindow::createFbo()" << endl;
    // The scene graph has been initialized. It is now time to create an FBO and associate
    // it with the QQuickWindow.
    _fbo = new QOpenGLFramebufferObject(size() * devicePixelRatio(), QOpenGLFramebufferObject::CombinedDepthStencil);
    _quickWindow->setRenderTarget(_fbo);
	cout << "done RenderControlWindow::createFbo()" << endl;
}

void RenderControlWindow::destroyFbo()
{
    delete _fbo;
    _fbo = nullptr;
}

void RenderControlWindow::run()
{
	cout << "RenderControlWindow::run()" << endl;
    disconnect(_qmlComponent, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(run()));

    if (_qmlComponent->isError()) {
        QList<QQmlError> errorList = _qmlComponent->errors();
		foreach(const QQmlError &error, errorList)
			cout << "errorUrl: " << error.url().url().toStdString() << " errorLine=" << error.line() << " errorMessage= " << error.description().toStdString() << endl;;
        return;
    }

    QObject *rootObject = _qmlComponent->create();
    if (_qmlComponent->isError()) {
        QList<QQmlError> errorList = _qmlComponent->errors();
        foreach (const QQmlError &error, errorList)
			cout << "errorUrl: " << error.url().url().toStdString() << " errorLine=" << error.line() << " errorMessage= " << error.description().toStdString() << endl;;
        return;
    }

    _rootItem = qobject_cast<QQuickItem *>(rootObject);
    if (!_rootItem) {
		cout << "run: Not a QQuickItem" << endl;
        delete rootObject;
        return;
    }

    // The root item is ready. Associate it with the window.
    _rootItem->setParentItem(_quickWindow->contentItem());

    // Update item and rendering related geometries.
    updateSizes();

    // Initialize the render control and our OpenGL resources.
    _openGLContext->makeCurrent(_offscreenSurface);
    _renderControl->initialize(_openGLContext);

    _openGLContext->doneCurrent();
    _quickInitialized = true;
	cout << "RenderControlWindow::run() finished" << endl;
}

void RenderControlWindow::updateSizes()
{
    // Behave like SizeRootObjectToView.
    _rootItem->setWidth(width());
    _rootItem->setHeight(height());

    _quickWindow->setGeometry(0, 0, width(), height());
}

void RenderControlWindow::startQuick(const QString &filename)
{
	cout << "Starting quick" << endl;
	_qmlComponent = new QQmlComponent(_qmlEngine, QUrl(filename));
	if (_qmlComponent->isLoading())
	{
		cout << "RenderControlWindow::startQuick isLoading()" << endl;
		connect(_qmlComponent, &QQmlComponent::statusChanged, this, &RenderControlWindow::run);
	}
	else
	{
		cout << "RenderControlWindow::startQuick Direct run" << endl;
		run();
	}
}

void RenderControlWindow::exposeEvent(QExposeEvent *)
{
	std::cout << "exposeEvent called." << endl;
    if (isExposed()) {
        render();
        if (!_quickInitialized)
            startQuick(QStringLiteral("qrc:/main.qml"));
    }
}

void RenderControlWindow::resizeEvent(QResizeEvent *)
{
    // If this is a resize after the scene is up and running, recreate the fbo and the
    // Quick item and scene.
    if (_rootItem && _openGLContext->makeCurrent(_offscreenSurface)) {
        delete _fbo;
        createFbo();
        _openGLContext->doneCurrent();
        updateSizes();
    }
}

bool RenderControlWindow::makeCurrent()
{
	if (!isReady())
		return false;

	return _openGLContext->makeCurrent(_offscreenSurface);
}

void RenderControlWindow::updateQuick()
{
	cout << "RenderControlWindow::updateQuick()" << endl;
    if (!_openGLContext->makeCurrent(_offscreenSurface))
        return;

	cout << "Made OpenGLContext current. " << endl;

    // Polish, synchronize and render the next frame (into our fbo).  In this example
    // everything happens on the same thread and therefore all three steps are performed
    // in succession from here. In a threaded setup the render() call would happen on a
    // separate thread.
	cout << "_renderControl->polishItems()" << endl;
    _renderControl->polishItems();
	cout << "_renderControl->sync()" << endl;
    _renderControl->sync();
	cout << "_renderControl->render()" << endl;
    _renderControl->render();

	cout << "_quickWindow->resetOpenGLState();" << endl;
    _quickWindow->resetOpenGLState();

	cout << "QOpenGLFramebufferObject::bindDefault();" << endl;
    QOpenGLFramebufferObject::bindDefault();

    _quickReady = true;

	cout << "render()" << endl;
    // Get something onto the screen.
    render();
}

void RenderControlWindow::render()
{
    if (!_openGLContext->makeCurrent(this))
        return;

    QOpenGLFunctions *f = _openGLContext->functions();
    f->glClearColor(0.0f, 0.1f, 0.25f, 1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_quickReady) {
        f->glFrontFace(GL_CW); // because our cube's vertex data is such
        f->glEnable(GL_CULL_FACE);
        f->glEnable(GL_DEPTH_TEST);

        f->glBindTexture(GL_TEXTURE_2D, _fbo->texture());

        static GLfloat angle = 0;
        QMatrix4x4 m;
        m.translate(0, 0, -2);
        m.rotate(90, 0, 0, 1);
        m.rotate(angle, 0.5, 1, 0);
        angle += 0.5f;

        f->glDisable(GL_DEPTH_TEST);
        f->glDisable(GL_CULL_FACE);
    }

    _openGLContext->swapBuffers(this);
}
