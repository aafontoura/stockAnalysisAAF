#include "indicatorholder.h"


IndicatorHolder::IndicatorHolder()
{

}

IndicatorHolder::IndicatorHolder(dataInterface *nIndicatorInterface)
{
    indicatorInterface = nIndicatorInterface;
    this->setName("Indicator");
    this->setType(SItemData::Indicator);
}

dataInterface *IndicatorHolder::getData()
{
    return this->indicatorInterface;
}
