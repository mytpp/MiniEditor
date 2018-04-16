#include "fucklistmodel.h"

fuckListModel::fuckListModel()
{
    shit = {1,2 ,15};
}
fuckListModel::~fuckListModel()
{}

int fuckListModel::rowCount(const QModelIndex &parent) const
{
    return shit.size();
}

int fuckListModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant fuckListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    return QVariant::fromValue(shit[row]);
}

QModelIndex fuckListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QModelIndex fuckListModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column, nullptr);
}
QHash<int, QByteArray> fuckListModel::roleNames() const
{
    QHash<int, QByteArray>  d;
    d[0] = "Foo";//给tpye1设置别名
    return  d;
}

