#ifndef ANGLEQMLRENDERSURFACE_H
#define ANGLEQMLRENDERSURFACE_H

#include <QQuickView>
#include <d3d9.h>
#include "angleqml_global.h"

class ANGLEQMLSHARED_EXPORT AngleQmlRenderSurface : public QQuickView
{
    Q_OBJECT

public:
    AngleQmlRenderSurface(QQmlEngine *engine, QWindow *parent);
    AngleQmlRenderSurface();

    IDirect3DSurface9 *getD3DSurfaceHandle();

    bool makeCurrent();

public slots:
    void handleOpenGLContextCreated(QOpenGLContext *context);
    void handleBeforeRendering();

private:
    bool _madeCurrent;
    void checkError();
    void createNewGLContext();
};

#endif // ANGLEQMLRENDERSURFACE_H
