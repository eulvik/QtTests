#include "namesviewmodel.h"

#include <time.h>

NamesViewModel::NamesViewModel(QObject *parent) :
    QAbstractListModel(parent)
{
    srand(time(NULL));
}

void NamesViewModel::addName(const DataItem &data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _dataItems.append(data);
    endInsertRows();

}

int NamesViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _dataItems.count();
}

QVariant NamesViewModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= _dataItems.count())
        return QVariant();

    const DataItem & data = _dataItems[index.row()];

    if(role == NameRole)
        return data.name();
    else if(role == ValueRole)
        return data.value();

    return QVariant();
}

QHash<int, QByteArray> NamesViewModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "names";
    roles[ValueRole] = "values";

    return roles;
}

void NamesViewModel::addNew()
{
    QString n = "Numero";
    int val = rand()%200;
    addName(DataItem(n, val));
}
