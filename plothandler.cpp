#include "plothandler.h"

PlotHandler::PlotHandler(QCustomPlot *plotter)
{
    this->setPlotter(plotter);
}

void PlotHandler::setPlotter(QCustomPlot *plotter)
{
    this->mainPlot = plotter;
}
