#include "angleglscene.h"

#include <QMouseEvent>
#include <math.h>
#include <iostream>

using namespace std;

AngleGLScene::AngleGLScene(QObject *parent) : QObject(parent),
    _angularSpeed(0),
    _glContext(NULL)
{
}

AngleGLScene::~AngleGLScene()
{
    _glContext->deleteTexture(_texture);
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

bool AngleGLScene::initializeGL(QGLContext *glContext)
{
    _glContext = glContext;
    initializeGLFunctions();
    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    _geometries.init();

    return true;
}

bool AngleGLScene::initShaders()
{
    // Compile vertex shader
    if (!_program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
        return false;

    // Compile fragment shader
    if (!_program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))
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
    glEnable(GL_TEXTURE_2D);
    _texture = _glContext->bindTexture(QImage(":/maria.png"));

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return true;
}

void AngleGLScene::resizeGL(int w, int h)
{
    // Set OpenGL viewport to cover whole widget
    glViewport(0, 0, w, h);

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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(_rotation);

    // Set modelview-projection matrix
    _program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    _program.setUniformValue("texture", 0);

    // Draw cube geometry
    _geometries.drawCubeGeometry(&_program);
}
