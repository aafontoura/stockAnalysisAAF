#include "sitem.h"
#include "stockprice.h"
#include "stockholder.h"
#include "indicatorholder.h"
#include "datainterface.h"

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
    if (this->m_itemData->getType()==SItemData::Stock)
    {
        if(child->getData()->getType()==SItemData::Indicator)
        {
            StockModel *st = (static_cast<StockHolder*>(m_itemData)->getSTTemp());
            dataInterface *indicator = static_cast<IndicatorHolder*>(child->getData())->getData();
            for(int i = 0; i < st->rowCount() ; i++)
            {

                indicator->addItem(st->data(i,StockModel::ClosePriceRole).toDouble(),st->data(i,StockModel::DateRole).toDateTime().toTime_t());
            }
        }
    }

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
    {
        int row = m_parentItem->m_childItems.indexOf(const_cast<SItem*>(this));
        return row;
    }
    return 0;
}

SItem *SItem::parentItem()
{
    return m_parentItem;
}
