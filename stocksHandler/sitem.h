#ifndef SITEM_H
#define SITEM_H

#include <QVariant>
#include "sitemdata.h"

class SItem
{
public:

    SItem(SItemData *data,SItem *parentItem = 0);
    SItem(SItem *parentItem = 0);
    ~SItem();

    void appendChild(SItem *child);
    void appendChild(SItemData *child);
    SItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    SItemData *getData();
    int row() const;
    SItem *parentItem();

private:
    QList<SItem*> m_childItems;
    SItemData *m_itemData;
    SItem *m_parentItem;

};

#endif // SITEM_H
