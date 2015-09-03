#ifndef QMLRENDERSURFACE_H
#define QMLRENDERSURFACE_H

#include <QQuickView>

#include "irendersurface.h"

class QmlRenderSurface : public QQuickView
{
    Q_OBJECT

public:
    QmlRenderSurface(const QUrl &source, QWindow *parent = 0);

    virtual IDirect3DSurface9 *getD3DSurfaceHandle();
    virtual bool makeCurrent();
};

#endif // QMLRENDERSURFACE_H
