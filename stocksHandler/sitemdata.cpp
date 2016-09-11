#include "sitemdata.h"

SItemData::SItemData()
{
    name = QString("Root");
    type = SItemDataType::Root;
}

SItemData::SItemData(const SItemData &myItemData)
{
    *this = myItemData;
}

QString SItemData::getName()
{
    return name;
}

SItemData::SItemDataType SItemData::getType()
{
    return type;
}

void SItemData::setName(QString newName)
{
    name = newName;
}

void SItemData::setType(SItemData::SItemDataType newType)
{
    type = newType;
}
