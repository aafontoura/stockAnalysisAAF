#include "plothandler.h"

PlotHandler::PlotHandler(QCustomPlot *plotter)
{
    this->setPlotter(plotter);
    connect(mainPlot,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(zoomChanged()));

}

void PlotHandler::setPlotter(QCustomPlot *plotter)
{
    this->mainPlot = plotter;
}

void PlotHandler::zoomChanged()
{
    emit zoomHasChanged();

}
