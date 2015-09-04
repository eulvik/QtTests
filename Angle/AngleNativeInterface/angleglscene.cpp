#include "angleglscene.h"

#include <QMouseEvent>
#include <math.h>
#include <iostream>

using namespace std;

AngleGLScene::AngleGLScene(QObject *parent) : QObject(parent),
    _geometries(0),
    _texture(0),
    _angularSpeed(0)
{
}

AngleGLScene::~AngleGLScene()
{
    //makeCurrent();
    delete _texture;
    delete _geometries;
    //doneCurrent();
}

bool AngleGLScene::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    return false;
}

bool AngleGLScene::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    _mousePressPosition = QVector2D(e->localPos());
    cout << "mousePress x:" << _mousePressPosition.x() << " y:" << _mousePressPosition.y() << endl;
    return true;
}

bool AngleGLScene::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - _mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    _rotationAxis = (_rotationAxis * _angularSpeed + n * acc).normalized();

    // Increase angular speed
    _angularSpeed += acc;

    printf("rotationAxis (%4.2f %4.2f %4.2f) angularSpeed:%4.2f", _rotationAxis.x(), _rotationAxis.y(), _rotationAxis.z(), _angularSpeed);

    cout << "mouseReleaseEvent()" << endl;

    return true;
}

bool AngleGLScene::mouseWheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
    return true;
}

void AngleGLScene::animate()
{
    cout << "animate()" << endl;
    _angularSpeed *= 0.99;

    if (_angularSpeed < 0.01) {
        _angularSpeed = 0.0;
    }
    else {
        _rotation = QQuaternion::fromAxisAndAngle(_rotationAxis, _angularSpeed) * _rotation;
    }
}

bool AngleGLScene::initializeGL()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    initShaders();
    initTextures();

    // Enable depth buffer
    gl->glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    gl->glEnable(GL_CULL_FACE);

    _geometries = new GeometryEngine();

    return true;
}

bool AngleGLScene::initShaders()
{
    // Compile vertex shader
    if (!_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        return false;

    // Compile fragment shader
    if (!_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        return false;

    // Link shader pipeline
    if (!_program.link())
        return false;

    // Bind shader pipeline for use
    if (!_program.bind())
        return false;

    return true;
}

bool AngleGLScene::initTextures()
{
    // Load cube.png image
    _texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

    // Set nearest filtering mode for texture minification
    _texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    _texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    _texture->setWrapMode(QOpenGLTexture::Repeat);

    return true;
}

void AngleGLScene::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void AngleGLScene::render()
{
    auto gl = QOpenGLContext::currentContext()->functions();
    // Clear color and depth buffer
    gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _texture->bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(_rotation);

    // Set modelview-projection matrix
    _program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    _program.setUniformValue("texture", 0);

    // Draw cube geometry
    _geometries->drawCubeGeometry(&_program);
}
