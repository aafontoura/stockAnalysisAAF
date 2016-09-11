#ifndef INDICATORHOLDER_H
#define INDICATORHOLDER_H


#include "sitemdata.h"
#include "../datainterface.h"

class IndicatorHolder : public SItemData
{
public:
    IndicatorHolder();
    IndicatorHolder(dataInterface *nIndicatorInterface);
    dataInterface *getData();
private:
    dataInterface *indicatorInterface;
};

#endif // INDICATORHOLDER_H
