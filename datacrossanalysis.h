#ifndef DATACROSSANALYSIS_H
#define DATACROSSANALYSIS_H

#include "datainterface.h"
#include "transactionorder.h"
#include "stockanalysis.h"
#include <QList>
#include <algorithm>

class dataCrossAnalysis : StockAnalysis
{
public:
    dataCrossAnalysis(dataInterface<qreal> *newContainerA, dataInterface<qreal> *newContainerB);    
    void analyseStock(void);
    QList<transactionOrder*> getTransactionOrderList();
    transactionOrder getLatestTransactionOrder();

private:
    dataInterface<qreal> *containerA;
    dataInterface<qreal> *containerB;

    QList<transactionOrder*> transactionOrderList;





};

#endif // DATACROSSANALYSIS_H
