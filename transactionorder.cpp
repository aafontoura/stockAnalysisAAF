#include "transactionorder.h"

transactionOrder::transactionOrder(stockStatus newStatus, qint32 newIndex)
{
    this->status = newStatus;
    this->index = newIndex;
    this->time = newIndex;
}

transactionOrder::transactionOrder(stockStatus newStatus, QDate newDate)
{
    this->status = newStatus;
    this->dateIndex = newDate;
    this->time = QDateTime(newDate).toTime_t();
}


transactionOrder::transactionOrder(stockStatus newStatus, double time)
{
    QDateTime qdtime;
    qdtime.setTime_t(time);

    this->status = newStatus;
    this->time = time;
    this->dateIndex = qdtime.date();
}

stockStatus transactionOrder::getStatus()
{
    return this->status;
}

QDate transactionOrder::getDate()
{
    return this->dateIndex;
}

qreal transactionOrder::getTime_t()
{
    return this->time;
}
