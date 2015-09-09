#ifndef ANGLEQMLRENDERSURFACE_H
#define ANGLEQMLRENDERSURFACE_H

#include <QQuickView>
#include <d3d9.h>
#include "angleqml_global.h"

class ANGLEQMLSHARED_EXPORT AngleQmlRenderWindow : public QQuickView
{
    Q_OBJECT

public:
    AngleQmlRenderWindow(QQmlEngine *engine, QWindow *parent);
    AngleQmlRenderWindow();

    IDirect3DSurface9 *getD3DSurfaceHandle();

    bool makeCurrent();

public slots:
    void handleBeforeRendering();

private:
    bool _madeCurrent;
    void checkError();
    void createNewGLContext();
};

#endif // ANGLEQMLRENDERSURFACE_H
