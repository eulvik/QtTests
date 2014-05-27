#ifndef MYSIMPLETEXTITEM_H
#define MYSIMPLETEXTITEM_H

#include <QGraphicsSimpleTextItem>
#include <QObject>

class MySimpleTextItem : public QObject, public QGraphicsSimpleTextItem
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ getRotationLocal WRITE setRotationLocal NOTIFY rotationChanged)
    Q_PROPERTY(qreal x READ getXLocal WRITE setXLocal NOTIFY xChanged)
    Q_PROPERTY(qreal y READ getYLocal WRITE setYLocal NOTIFY yChanged)

public:
    explicit MySimpleTextItem(const QString& text);
    qreal getRotationLocal();
    void setRotationLocal(qreal angle);
    qreal getXLocal();
    void setXLocal(qreal x);
    qreal getYLocal();
    void setYLocal(qreal y);
signals:
    void rotationChanged();
    void xChanged();
    void yChanged();

public slots:

private:
    qreal rotation;
};

#endif // MYSIMPLETEXTITEM_H
