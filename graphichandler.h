#ifndef GRAPHICHANDLER_H
#define GRAPHICHANDLER_H

#include "qcustomplot.h"

class GraphicHandler
{
public:
    GraphicHandler();
    QCustomPlot *getPlotHandler();
private:
    QCustomPlot *gHandler;

};

#endif // GRAPHICHANDLER_H
