#ifndef PLOTHANDLER_H
#define PLOTHANDLER_H

#include "qcustomplot.h"


class PlotHandler
{
public:
    PlotHandler(QCustomPlot *plotter);
    void setPlotter(QCustomPlot *plotter);

private:

    QCustomPlot *mainPlot;
};

#endif // PLOTHANDLER_H
