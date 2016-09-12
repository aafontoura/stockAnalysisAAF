#ifndef SMODELVIEW_H
#define SMODELVIEW_H


#include "graphichandler.h"
//#include "stocksHandler/sitemdata.h"
#include "stocksHandler/sitem.h"
#include "stocksHandler/stockholder.h"
#include "QList"
#include "datainterface.h"

class SModelView : public QObject
{
    Q_OBJECT

public:
    SModelView();
    SModelView(GraphicHandler *plotHandler,SItemData *displayData = 0);
    void setStockData(SItem *sData);
    StockHolder *getStockData();


    QCustomPlot *getGraphDrawer();

    const uint C_ONE_DAY_T = 60*60*24;

private slots:
    void zoomChanged();

private:
    GraphicHandler *plotHandler;
    StockHolder *displayData;
    QList<dataInterface*> data;

    QVector<double> axisVector;
    QVector<QString> axisVectorLabel;

};

#endif // SMODELVIEW_H

