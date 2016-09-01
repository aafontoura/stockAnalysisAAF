#include "stockholder.h"

StockHolder::StockHolder(StockModel *newSt)
{
    this->setType(SItemDataType::Stock);
    this->setName(newSt->stockName());
    st = newSt;

}

StockModel *StockHolder::getSTTemp()
{
    return this->st;
}
