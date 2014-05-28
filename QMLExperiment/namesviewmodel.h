#ifndef NAMESVIEWMODEL_H
#define NAMESVIEWMODEL_H

#include <QList>
#include <QAbstractListModel>

#include "dataitem.h"

class NamesViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    NamesViewModel(QObject *parent = 0);

    enum DisplayRoles{
        NameRole = Qt::UserRole + 1,
        ValueRole
    };

    void addName(const DataItem &data);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;


signals:

public slots:
    void addNew();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<DataItem> _dataItems;
};

#endif // NAMESVIEWMODEL_H
