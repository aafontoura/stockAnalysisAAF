#ifndef STOCKANALYSIS_H
#define STOCKANALYSIS_H

#include "transactionorder.h"

class StockAnalysis
{
public:
    StockAnalysis();
    virtual void analyseStock(void) = 0;
    virtual QList<transactionOrder*> getTransactionOrderList() = 0;
    virtual transactionOrder getLatestTransactionOrder() = 0;
};

#endif // STOCKANALYSIS_H
