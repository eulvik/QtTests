#include "dataitem.h"

DataItem::DataItem(QString name, int value)
{
    _name = name;
    _value = value;
}

QString DataItem::name() const
{
    return _name;
}

void DataItem::setName(QString name)
{
    _name = name;
}

int DataItem::value() const
{
    return _value;
}

void DataItem::setValue(int value)
{
    _value = value;
}
