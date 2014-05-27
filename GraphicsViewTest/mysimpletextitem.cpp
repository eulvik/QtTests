#include "mysimpletextitem.h"

MySimpleTextItem::MySimpleTextItem(const QString& text) :
    QGraphicsSimpleTextItem(text)
{
}

qreal MySimpleTextItem::getRotationLocal()
{
    return QGraphicsSimpleTextItem::rotation();
}

void MySimpleTextItem::setRotationLocal(qreal angle)
{
    this->setRotation(angle);
    rotationChanged();
}

qreal MySimpleTextItem::getXLocal()
{
    return this->x();
}

void MySimpleTextItem::setXLocal(qreal x)
{
   setX(x);
   xChanged();
}

qreal MySimpleTextItem::getYLocal()
{
    return this->y();
}

void MySimpleTextItem::setYLocal(qreal y)
{
   setY(y);
   yChanged();
}
