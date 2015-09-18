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
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QQmlContext>
#include <iostream>

using namespace std;

RenderControlWindow::RenderControlWindow(QWindow *parent)
	: QWindow(parent),
	_rotation(0),
	_rootItem(nullptr),
	_fbo(nullptr),
    _quickInitialized(false),
    _quickReady(false)
{
    cout << "Constructor RenderControlWindow." << endl;
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
	_offscreenSurface->setFormat(_openGLContext->format());

	cout << "_offscreenSurface->create()" << endl;
	_offscreenSurface->create();

	cout << "Constructing QQuickRenderControl." << endl;
	_renderControl = new QQuickRenderControl(this);

	cout << "Constructing QQuickWindow" << endl;
	_quickWindow = new QQuickWindow(_renderControl);

	cout << "Constructing QQmlEngine" << endl;
	_qmlEngine = new QQmlEngine;
	if (!_qmlEngine->incubationController())
		_qmlEngine->setIncubationController(_quickWindow->incubationController());

	_qmlEngine->rootContext()->setContextProperty("vm", this);
    _updateTimer.setInterval(16);
    _updateTimer.setSingleShot(true);
    connect(&_updateTimer, &QTimer::timeout, this, &RenderControlWindow::realUpdateQuick);

	cout << "Connecting signals/slots" << endl;
	connect(_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &RenderControlWindow::createFbo);
	connect(_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &RenderControlWindow::destroyFbo);
	//Tror ikke vi trenger disse ?
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

	_fbo = new QOpenGLFramebufferObject(size() * devicePixelRatio(), QOpenGLFramebufferObject::CombinedDepthStencil);
	_quickWindow->setRenderTarget(_fbo);
    //This makes the output not visible, but content will be put in backbuffer.
	setVisible(false);
}

void RenderControlWindow::destroyFbo()
{
	delete _fbo;
    _fbo = nullptr;
}

void RenderControlWindow::realUpdateQuick()
{
    if(!isReady())
        return;

    if (!_openGLContext->makeCurrent(_offscreenSurface))
        return;

    _renderControl->polishItems();
    _renderControl->sync();
    _renderControl->render();

    _quickWindow->resetOpenGLState();

    QOpenGLFramebufferObject::bindDefault();

    _quickReady = true;

    render();
}

void RenderControlWindow::runQuickRendering()
{
	cout << "RenderControlWindow::run()" << endl;
	disconnect(_qmlComponent, SIGNAL(statusChanged(QQmlComponent::Status)), this, SLOT(run()));

    if(_quickInitialized)
        return;

	if (_qmlComponent->isError()) {
		QList<QQmlError> errorList = _qmlComponent->errors();
		foreach(const QQmlError &error, errorList)
			cout << "errorUrl: " << error.url().url().toStdString() << " errorLine=" << error.line() << " errorMessage= " << error.description().toStdString() << endl;;
		return;
	}

	QObject *rootObject = _qmlComponent->create();
	if (_qmlComponent->isError()) {
		QList<QQmlError> errorList = _qmlComponent->errors();
		foreach(const QQmlError &error, errorList)
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

    _shaderProgram = new QOpenGLShaderProgram;
    _shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, readTextFromFile(":/vertexshader.vsh"));

    _shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, readTextFromFile(":/fragmentshader.fsh"));
	_shaderProgram->bindAttributeLocation("vertex", 0);
	_shaderProgram->bindAttributeLocation("coord", 1);
	_shaderProgram->link();
	cout << "Error: " << _shaderProgram->log().toStdString() << endl;

	_matrixLoc = _shaderProgram->uniformLocation("matrix");

	_vertexArrayObject = new QOpenGLVertexArrayObject;
	_vertexArrayObject->create();
	_vertexArrayObject->bind();

	_vertexBufferObject = new QOpenGLBuffer;
	_vertexBufferObject->create();
	_vertexBufferObject->bind();

	GLfloat v[] = {
		-0.5, -0.5, 0.5,
		-0.5, 0.5, 0.5,
		0.5, -0.5, 0.5,

		0.5,-0.5, 0.5,
		-0.5, 0.5, 0.5,
		0.5, 0.5, 0.5
	};
	GLfloat texCoords[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,

		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	const int vertexCount = 6;
	_vertexBufferObject->allocate(sizeof(GLfloat) * vertexCount * 5);
	_vertexBufferObject->write(0, v, sizeof(GLfloat) * vertexCount * 3);
	_vertexBufferObject->write(sizeof(GLfloat) * vertexCount * 3, texCoords, sizeof(GLfloat) * vertexCount * 2);
	_vertexBufferObject->release();

	if (_vertexArrayObject->isCreated())
		setupVertexAttribs();

	// Must unbind before changing the current context. Hence the absence of
	// QOpenGLVertexArrayObject::Binder here.
	_vertexArrayObject->release();


	// Initialize the render control and our OpenGL resources.
	_openGLContext->makeCurrent(_offscreenSurface);

	_openGLContext->doneCurrent();
	_quickInitialized = true;
	cout << "RenderControlWindow::run() finished" << endl;
}

void RenderControlWindow::updateSizes()
{
	_rootItem->setWidth(width());
	_rootItem->setHeight(height());

	_projMatrix.setToIdentity();

	_quickWindow->setGeometry(0, 0, width(), height());
}

void RenderControlWindow::setupVertexAttribs()
{
	_vertexBufferObject->bind();
	_shaderProgram->enableAttributeArray(0);
	_shaderProgram->enableAttributeArray(1);
	_openGLContext->functions()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    _openGLContext->functions()->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void *)(6 * 3 * sizeof(GLfloat)));
    _vertexBufferObject->release();
}

QString RenderControlWindow::readTextFromFile(const QString &path)
{
    QFile file(path);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream fileStream(&file);
    QString text = fileStream.readAll();
    cout << path.toStdString() << " " << text.toStdString() << endl;

    return text;
}

float RenderControlWindow::rotation() const
{
    return _rotation;
}

void RenderControlWindow::setRotation(float rotation)
{
    if(rotation > 360)
        _rotation = 0;
    else
        _rotation = rotation;

    rotationChanged();
}

void RenderControlWindow::startQuick(const QString &filename)
{
    _qmlComponent = new QQmlComponent(_qmlEngine, QUrl(filename));
	if (_qmlComponent->isLoading())
		connect(_qmlComponent, &QQmlComponent::statusChanged, this, &RenderControlWindow::runQuickRendering);
	else
		runQuickRendering();
}

void RenderControlWindow::exposeEvent(QExposeEvent *)
{
	if (isExposed()) {
        render();
		if (!_quickInitialized)
			startQuick(QStringLiteral("qrc:/main.qml"));
	}
}

void RenderControlWindow::resizeEvent(QResizeEvent *)
{
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

	return _openGLContext->makeCurrent(this);
}

void RenderControlWindow::updateQuick()
{
    if(!_updateTimer.isActive())
        _updateTimer.start();
}

void RenderControlWindow::render()
{
	if (!makeCurrent())
		return;

	QOpenGLFunctions *f = _openGLContext->functions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (_quickReady) {
		f->glFrontFace(GL_CW);
		f->glEnable(GL_CULL_FACE);
		f->glEnable(GL_DEPTH_TEST);

		f->glBindTexture(GL_TEXTURE_2D, _fbo->texture());

		_shaderProgram->bind();
		QOpenGLVertexArrayObject::Binder vaoBinder(_vertexArrayObject);
		// If VAOs are not supported, set the vertex attributes every time.
		if (!_vertexArrayObject->isCreated())
			setupVertexAttribs();

		_shaderProgram->setUniformValue(_matrixLoc, _projMatrix);

		f->glDrawArrays(GL_TRIANGLES, 0, 6);

		f->glDisable(GL_DEPTH_TEST);
		f->glDisable(GL_CULL_FACE);
	}

	_openGLContext->swapBuffers(this);
}
