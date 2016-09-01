#include "sitem.h"
#include "sitemdata.h"
#include "sitemtreemodel.h"

#include <QStringList>

SItemTreeModel::SItemTreeModel(const QString &data, QObject *parent)
{
    rootItem = new SItem();
}

SItemTreeModel::SItemTreeModel(QObject *parent)
{
     rootItem = new SItem();
}

SItemTreeModel::SItemTreeModel(SItem *data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = data;

}

SItemTreeModel::~SItemTreeModel()
{
    delete rootItem;
}

int SItemTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<SItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

void SItemTreeModel::addItem(SItem *nItem, const QModelIndex &parent)
{
    beginInsertRows(parent, rootItem->childCount(), rootItem->childCount()+1);
    rootItem->appendChild(nItem);
    endInsertRows();

}

SItem *SItemTreeModel::getItem(const QModelIndex &index)
{
    if (!index.isValid())
        return NULL;


    SItem *item = static_cast<SItem*>(index.internalPointer());

    return item;



}

bool SItemTreeModel::insertRow(int row, const QModelIndex &parent)
{
    //SItem *parentItem = getItem(parent);
    bool success;

    //beginInsertRows(parent, position, position + rows - 1);
    //success = parentItem->appendChild();
    //endInsertRows();

    return success;
}


QVariant SItemTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();


    if (role != Qt::DisplayRole)
        return QVariant();

    SItem *item = static_cast<SItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags SItemTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant SItemTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

SItem *SItemTreeModel::getHeader()
{
    return rootItem;
}

QModelIndex SItemTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    SItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SItem*>(parent.internalPointer());

    SItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex SItemTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    SItem *childItem = static_cast<SItem*>(index.internalPointer());
    SItem *parentItem = childItem->parentItem();

    if ((parentItem == rootItem)||(NULL == parentItem))
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int SItemTreeModel::rowCount(const QModelIndex &parent) const
{
    SItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SItem*>(parent.internalPointer());

    return parentItem->childCount();
}

