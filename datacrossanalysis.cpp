#include "datacrossanalysis.h"
#include <QtCore/QDebug>

dataCrossAnalysis::dataCrossAnalysis(dataInterface<qreal> *newContainerA, dataInterface<qreal> *newContainerB)
{
    containerA = newContainerA;
    containerB = newContainerB;
}

void dataCrossAnalysis::analyseStock(void)
{
    if ((this->containerA != NULL) &&
        (this->containerB != NULL))
    {

        QList<double> time;
        qreal startTime;

        /* Concatenate both time vectors and sort */
        time.append(containerA->getTimeVector().toList());
        time.append(containerB->getTimeVector().toList());
        std::sort(time.begin(), time.end());


        /* Remove duplicated entries */
        for (int i = 1; i<time.size();i++)
        {
            while (i<time.size()&&(time.at(i)==time.at(i-1)))
            {

                time.removeAt(i);
            }
        }

        startTime = containerA->getTimeVector().at(0);
        if (containerB->getTimeVector().at(0) > startTime)
        {
            startTime = containerB->getTimeVector().at(0);
        }

        bool previousStatus = containerA->getValueTime(startTime) > containerB->getValueTime(startTime);

        for (int i = time.indexOf(startTime)+1; i<time.size(); i++)
        {
            bool currentStatus;
            currentStatus = containerA->getValueTime(time.at(i)) > containerB->getValueTime(time.at(i));

            if (currentStatus != previousStatus)
            {
                previousStatus = currentStatus;
                if(true == currentStatus)
                {
                    /* Buy */
                    transactionOrderList.push_back(new transactionOrder(BOUGHT_STOCK,time.at(i)));
                }
                else
                {
                    /* Sell */
                    transactionOrderList.push_back(new transactionOrder(SELLED_STOCK,time.at(i)));
                }

            }
        }



    }
}

QList<transactionOrder *> dataCrossAnalysis::getTransactionOrderList()
{
    return this->transactionOrderList;
}

transactionOrder dataCrossAnalysis::getLatestTransactionOrder()
{
    return *(transactionOrderList.last());
}


