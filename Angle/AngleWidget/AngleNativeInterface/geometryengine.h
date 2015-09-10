#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();

    virtual ~GeometryEngine();
    void init();
    void drawCubeGeometry(QOpenGLShaderProgram *program);

private:
    void initCubeGeometry();

    GLuint vboIds[2];
};

#endif // GEOMETRYENGINE_H
