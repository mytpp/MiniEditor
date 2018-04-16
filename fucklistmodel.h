#ifndef FUCKLISTMODEL_H
#define FUCKLISTMODEL_H

#include <QAbstractListModel>
#include <vector>


//for debug
class fuckListModel: public QAbstractListModel
{
    Q_OBJECT
public:
    fuckListModel();
    fuckListModel(QObject *parent);
    ~fuckListModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames()  const;//返回数据别名

//Q_INVOKABLE void testFunction(){
//    emit testSlot(5);
//}
private:
    std::vector<int> shit;

//signals:
//    void testSlot(int event);
};

#endif // FUCKLISTMODEL_H
