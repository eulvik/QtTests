#ifndef ANGLEGLWIDGET_H
#define ANGLEGLWIDGET_H

#include <QGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "geometryengine.h"

class AngleGLScene : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit AngleGLScene(QObject* parent = 0);
    ~AngleGLScene();

    bool mouseMoveEvent(QMouseEvent *e);
    bool mousePressEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool mouseWheelEvent(QWheelEvent *e);

    bool initializeGL(QOpenGLContext *glContext);
    void resizeGL(int w, int h);
    void render();
    void animate();

private:
    bool initShaders();
    bool initTextures();
    QOpenGLShaderProgram _program;
    GeometryEngine _geometries;
    QOpenGLContext* _glContext;
    QOpenGLTexture* _texture;

    QMatrix4x4 projection;

    QVector2D _mousePressPosition;
    QVector3D _rotationAxis;
    qreal _angularSpeed;
    QQuaternion _rotation;
};

#endif // ANGLEGLWIDGET_H
