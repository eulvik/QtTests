#ifndef DATAITEM_H
#define DATAITEM_H

#include <QString>

class DataItem
{
public:
    DataItem(QString name, int value);
    QString name() const;
    void setName(QString name);
    int value() const;
    void setValue(int value);

private:
    QString _name;
    int _value;
};

#endif // DATAITEM_H
