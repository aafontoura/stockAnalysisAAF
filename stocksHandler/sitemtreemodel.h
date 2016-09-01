#ifndef SITEMTREEMODEL_H
#define SITEMTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "sitem.h"

class SItemTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    enum SItemNodeRole
    {
        StockData = Qt::UserRole
    };

    SItemTreeModel(SItem *data, QObject *parent = 0);
    SItemTreeModel(const QString &data, QObject *parent = 0);
    SItemTreeModel(QObject *parent = 0);
    ~SItemTreeModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    SItem *getHeader();
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    void addItem(SItem *nItem, const QModelIndex &parent = QModelIndex());
    SItem *getItem(const QModelIndex &index);
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());

private:
    //void setupModelData(const QStringList &lines, TreeItem *parent);

    SItem *rootItem;
};

#endif // SITEMTREEMODEL_H
