#include "mysimpletextitem.h"

#include <QFont>

#include <iostream>
using namespace std;

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

qreal MySimpleTextItem::getScaleLocal()
{
    return font().pointSizeF();
}

void MySimpleTextItem::setScaleLocal(qreal scale)
{
    cout << "setScaleLocal " << scale << endl;
    this->font().setPointSizeF(scale);
    scaleChanged();
}
