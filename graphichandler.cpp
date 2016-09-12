#include "graphichandler.h"

GraphicHandler::GraphicHandler()
{
    this->gHandler = new QCustomPlot();
    connect(this->gHandler,SIGNAL(mouseWheel(QWheelEvent*)),this,SLOT(zoomChanged()));
}

QCustomPlot *GraphicHandler::getPlotHandler()
{
    return this->gHandler;
}


void GraphicHandler::zoomChanged()
{
    emit zoomHasChanged();

}
