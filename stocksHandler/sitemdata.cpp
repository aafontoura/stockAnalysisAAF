#include "sitemdata.h"

SItemData::SItemData()
{
    name = QString("Root");
    type = SItemDataType::Root;
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
