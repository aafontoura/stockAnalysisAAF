#ifndef BALANCEANALYSIS_H
#define BALANCEANALYSIS_H


#include "transactionorder.h"
#include "stockprice.h"

class BalanceAnalysis
{
public:
    BalanceAnalysis(QList<transactionOrder*> orders, StockModel *stock);
    double analyseBalance(QDate begin, QDate end);
private:
    QList<transactionOrder*> historicOrders;
    StockModel *stockHistoric;
    QList<double> historicGain;
    QList<double> time;

    qreal getNextOpenStockPrice(QDate date);
    QDate getNextOpenStockDate(QDate date);

};

#endif // BALANCEANALYSIS_H
