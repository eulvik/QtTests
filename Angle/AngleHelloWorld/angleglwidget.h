#ifndef ANGLEGLWIDGET_H
#define ANGLEGLWIDGET_H

#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QGLShaderProgram>

#include "geometryengine.h"

class AngleGLScene : public QGLWidget, protected QGLFunctions
{
    Q_OBJECT
public:
    explicit AngleGLScene(QWidget *parent = 0);
    ~AngleGLScene();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void simulate(QTimerEvent *e);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initTextures();

private:
    QBasicTimer timer;
    QGLShaderProgram program;
    GeometryEngine geometries;

    GLuint texture;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed;
    QQuaternion rotation;
};

#endif // ANGLEGLWIDGET_H
