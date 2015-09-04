#ifndef ANGLEGLWIDGET_H
#define ANGLEGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "geometryengine.h"

class AngleGLScene : public QObject
{
    Q_OBJECT

public:
    explicit AngleGLScene(QObject* parent = 0);
    ~AngleGLScene();

    bool mouseMoveEvent(QMouseEvent *e);
    bool mousePressEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool mouseWheelEvent(QWheelEvent *e);

    bool initializeGL();
    void resizeGL(int w, int h);
    void render();
    void animate();

private:
    bool initShaders();
    bool initTextures();
    QBasicTimer _timer;
    QOpenGLShaderProgram _program;
    GeometryEngine *_geometries;

    QOpenGLTexture *_texture;

    QMatrix4x4 projection;

    QVector2D _mousePressPosition;
    QVector3D _rotationAxis;
    qreal _angularSpeed;
    QQuaternion _rotation;
};

#endif // ANGLEGLWIDGET_H
