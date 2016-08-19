#include "balanceanalysis.h"
#include <QtCore/QDebug>




BalanceAnalysis::BalanceAnalysis(QList<transactionOrder *> orders, StockModel *stock)
{
    int i;

    historicOrders = orders;
    stockHistoric = stock;

    /* Ignore if first order is sell */
    /*for (i = 0 ; i < orders.size() && (orders[i]->getStatus() == SELLED_STOCK); i++)
    {
        historicGain.append(1.0);
        time.append(0.0);
    }


    double boughtValue=0;
    bool stockBought = false;
    for (; i<orders.size() ; i++)
    {
        if (orders[i]->getStatus() == BOUGHT_STOCK)
        {
            if (false == stockBought)
            {
                /* buying the next day opening *//*
                boughtValue = this->getNextOpenStockPrice(orders[i]->getDate());
                stockBought = true;
            }
            historicGain.append(historicGain.last());
        }
        else
        {
            if (true == stockBought)
            {
                /* selling in the next day opening *//*
                historicGain.append(historicGain.last() * (this->getNextOpenStockPrice(orders[i]->getDate()) / boughtValue));
            }
            else
            {
                historicGain.append(historicGain.last());
            }
        }

        time.append(getNextOpenStockDate(orders[i]->getDate()));
    }
*/
}

QDate BalanceAnalysis::getNextOpenStockDate(QDate date)
{
    switch (stockHistoric->dataCycle())
    {
    case StockModel::Daily:
        date = date.addDays(1);
        break;
    case StockModel::Weekly:
        date = date.addDays(7);
        break;

    case StockModel::Monthly:
        date = date.addMonths(1);
        break;

    default:
        break;

    }

    return date;
}

qreal BalanceAnalysis::getNextOpenStockPrice(QDate date)
{
    return stockHistoric->data(getNextOpenStockDate(date),StockModel::OpenPriceRole).toDouble();
}

double BalanceAnalysis::analyseBalance(QDate begin, QDate end)
{
    int i ;

    for (i = 0 ; i < historicOrders.size() && (historicOrders[i]->getDate() < begin); i++);

    for (; i < historicOrders.size() && (historicOrders[i]->getStatus() == SELLED_STOCK); i++);

    double boughtValue=0;
    bool stockBought = false;
    double gain = 1;
    for (; i<historicOrders.size() ; i++)
    {
        if (historicOrders[i]->getStatus() == BOUGHT_STOCK)
        {
            if (false == stockBought)
            {
                /* buying the next day opening */
                boughtValue = this->getNextOpenStockPrice(historicOrders[i]->getDate());
                stockBought = true;
            }
        }
        else
        {
            if (true == stockBought)
            {
                /* selling in the next day opening */
                gain *= (this->getNextOpenStockPrice(historicOrders[i]->getDate()) / boughtValue);
                stockBought = false;
            }
        }
    }

    if (true == stockBought)
    {
        gain *= stockHistoric->data(end,StockModel::ClosePriceRole).toDouble() / boughtValue;
    }

    qDebug() << "ganho = " << (gain*100.0);

    return gain;
}


