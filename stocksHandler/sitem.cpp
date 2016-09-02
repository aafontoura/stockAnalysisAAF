#include "sitem.h"

SItem ::SItem(SItemData *data, SItem *parentItem)
{
    m_parentItem = parentItem;
    m_itemData = data;
}

SItem::SItem(SItem *parentItem)
{
    m_parentItem = parentItem;
    m_itemData = new SItemData();
}

SItem::~SItem()
{
    qDeleteAll(m_childItems);
}

void SItem::appendChild(SItem *child)
{
    m_childItems.append(child);


}

void SItem::appendChild(SItemData *child)
{
    m_childItems.append(new SItem(child,this));
}

SItem *SItem::child(int row)
{
    return m_childItems.value(row);
}

int SItem::childCount() const
{
    return m_childItems.count();
}

int SItem::columnCount() const
{
    /*return m_itemData.count();*/
    return 1;
}

QVariant SItem::data(int column) const
{
    return m_itemData->getName();
}

SItemData *SItem::getData()
{
    return m_itemData;
}

int SItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<SItem*>(this));

    return 0;
}

SItem *SItem::parentItem()
{
    return m_parentItem;
}
