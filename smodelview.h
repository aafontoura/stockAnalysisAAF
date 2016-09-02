#ifndef SMODELVIEW_H
#define SMODELVIEW_H


#include "graphichandler.h"
//#include "stocksHandler/sitemdata.h"
#include "stocksHandler/sitem.h"
#include "stocksHandler/stockholder.h"

class SModelView
{
public:
    SModelView();
    SModelView(GraphicHandler *plotHandler,SItemData *displayData = 0);
    void setStockData(SItem *sData);
    StockHolder *getStockData();


    QCustomPlot *getGraphDrawer();

private:
    GraphicHandler *plotHandler;
    StockHolder *displayData;

};

#endif // SMODELVIEW_H

