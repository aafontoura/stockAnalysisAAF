#ifndef STOCKHOLDER_H
#define STOCKHOLDER_H

#include "sitemdata.h"
#include "../stockprice.h"
class StockHolder : public SItemData
{
public:
    StockHolder(StockModel *newSt);
    StockModel *getSTTemp();

private:
    StockModel *st;
};

#endif // STOCKHOLDER_H
