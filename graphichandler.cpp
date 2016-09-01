#include "graphichandler.h"

GraphicHandler::GraphicHandler()
{
    this->gHandler = new QCustomPlot();
}

QCustomPlot *GraphicHandler::getPlotHandler()
{
    return this->gHandler;
}
