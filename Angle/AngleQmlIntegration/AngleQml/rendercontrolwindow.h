#ifndef RENDERCONTROLWINDOW_H
#define RENDERCONTROLWINDOW_H

#include <QWindow>
#include <QTimer>

#include "angleqml_global.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLFramebufferObject)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)
QT_FORWARD_DECLARE_CLASS(QOffscreenSurface)
QT_FORWARD_DECLARE_CLASS(QQuickRenderControl)
QT_FORWARD_DECLARE_CLASS(QQuickWindow)
QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QQmlComponent)
QT_FORWARD_DECLARE_CLASS(QQuickItem)

class ANGLEQMLSHARED_EXPORT RenderControlWindow : public QWindow
{
    Q_OBJECT

public:
    RenderControlWindow();
    ~RenderControlWindow();

    void startQuick(const QString &filename);
    void updateQuick();

protected:
    void exposeEvent(QExposeEvent *e) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private slots:
    void render();
    void run();
    void createFbo();
    void destroyFbo();

private:
    void updateSizes();

    QOpenGLContext *_openGLContext;
    QOffscreenSurface *_offscreenSurface;
    QQuickRenderControl *_renderControl;
    QQuickWindow *_quickWindow;
    QQmlEngine *_qmlEngine;
    QQmlComponent *_qmlComponent;
    QQuickItem *_rootItem;
    QOpenGLFramebufferObject *_fbo;
    bool _quickInitialized;
    bool _quickReady;
};

#endif // RENDERCONTROLWINDOW_H
