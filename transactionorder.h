#ifndef TRANSACTIONORDER_H
#define TRANSACTIONORDER_H

#include "AAFAnalysisDefs.h"
#include <QDate>


class transactionOrder
{
public:
    transactionOrder(stockStatus newStatus, qint32 newIndex);
    transactionOrder(stockStatus newStatus, QDate newDate);
    transactionOrder(stockStatus newStatus, double time);
    stockStatus getStatus();
    QDate getDate();
    qreal getTime_t();

private:
    stockStatus status;
    qint32 index;
    QDate dateIndex;
    double time;

};

#endif // TRANSACTIONORDER_H
