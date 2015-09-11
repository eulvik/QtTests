#include "rendercontrolwindow.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QCoreApplication>

RenderControlWindow::RenderControlWindow()
    : _rootItem(0),
      _fbo(0),
      _quickInitialized(false),
      _quickReady(false)
{
    setSurfaceType(QSurface::OpenGLSurface);

    QSurfaceFormat format;
    // Qt Quick may need a depth and stencil buffer. Always make sure these are available.
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
    setFormat(format);

    _openGLContext = new QOpenGLContext;
    _openGLContext->setFormat(format);
    _openGLContext->create();

    _offscreenSurface = new QOffscreenSurface;
    // Pass m_context->format(), not format. Format does not specify and color buffer
    // sizes, while the context, that has just been created, reports a format that has
    // these values filled in. Pass this to the offscreen surface to make sure it will be
    // compatible with the context's configuration.
    _offscreenSurface->setFormat(_openGLContext->format());
    _offscreenSurface->create();

    _renderControl = new QQuickRenderControl(this);

    // Create a QQuickWindow that is associated with out render control. Note that this
    // window never gets created or shown, meaning that it will never get an underlying
    // native (platform) window.
    _quickWindow = new QQuickWindow(_renderControl);

    // Create a QML engine.
    _qmlEngine = new QQmlEngine;
    if (!_qmlEngine->incubationController())
        _qmlEngine->setIncubationController(_quickWindow->incubationController());

    // Now hook up the signals. For simplicy we don't differentiate between
    // renderRequested (only render is needed, no sync) and sceneChanged (polish and sync
    // is needed too).
    connect(_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &RenderControlWindow::createFbo);
    connect(_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &RenderControlWindow::destroyFbo);
    connect(_renderControl, &QQuickRenderControl::renderRequested, this, &RenderControlWindow::updateQuick);
    connect(_renderControl, &QQuickRenderControl::sceneChanged, this, &RenderControlWindow::updateQuick);
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
    // The scene graph has been initialized. It is now time to create an FBO and associate
    // it with the QQuickWindow.
    _fbo = new QOpenGLFramebufferObject(size() * devicePixelRatio(), QOpenGLFramebufferObject::CombinedDepthStencil);
    _quickWindow->setRenderTarget(_fbo);
}

void RenderControlWindow::destroyFbo()
{
    delete _fbo;
    _fbo = 0;
}

void RenderControlWindow::run()
{
    disconnect(_qmlComponent, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(run()));

    if (_qmlComponent->isError()) {
        QList<QQmlError> errorList = _qmlComponent->errors();
        foreach (const QQmlError &error, errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    QObject *rootObject = _qmlComponent->create();
    if (_qmlComponent->isError()) {
        QList<QQmlError> errorList = _qmlComponent->errors();
        foreach (const QQmlError &error, errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    _rootItem = qobject_cast<QQuickItem *>(rootObject);
    if (!_rootItem) {
        qWarning("run: Not a QQuickItem");
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
    _qmlComponent = new QQmlComponent(_qmlEngine, QUrl(filename));
    if (_qmlComponent->isLoading())
        connect(_qmlComponent, &QQmlComponent::statusChanged, this, &RenderControlWindow::run);
    else
        run();
}

void RenderControlWindow::exposeEvent(QExposeEvent *)
{
    if (isExposed()) {
        render();
        if (!_quickInitialized)
            startQuick(QStringLiteral("qrc:/rendercontrol/demo.qml"));
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

void RenderControlWindow::updateQuick()
{
    if (!_openGLContext->makeCurrent(_offscreenSurface))
        return;

    // Polish, synchronize and render the next frame (into our fbo).  In this example
    // everything happens on the same thread and therefore all three steps are performed
    // in succession from here. In a threaded setup the render() call would happen on a
    // separate thread.
    _renderControl->polishItems();
    _renderControl->sync();
    _renderControl->render();

    _quickWindow->resetOpenGLState();
    QOpenGLFramebufferObject::bindDefault();

    _quickReady = true;

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
