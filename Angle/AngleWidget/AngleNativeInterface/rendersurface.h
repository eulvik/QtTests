#ifndef RENDERSURFACE_H
#define RENDERSURFACE_H

#include <QOpenGLFunctions>
#include <QWindow>
#include <d3d9.h>

class RenderSurface : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

protected:

    QOpenGLContext *_context;
    bool createGraphicsContext();

public:
    RenderSurface(QWindow* parent);
    IDirect3DSurface9 *getD3DSurfaceHandle();

    bool makeCurrent();
    QOpenGLContext*  glContext() { return _context; }

};

#endif // RENDERSURFACE_H
