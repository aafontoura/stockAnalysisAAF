#include "indicatorholder.h"


IndicatorHolder::IndicatorHolder()
{

}

IndicatorHolder::IndicatorHolder(dataInterface *nIndicatorInterface)
{
    indicatorInterface = nIndicatorInterface;
    this->setName("Indicator");
}
