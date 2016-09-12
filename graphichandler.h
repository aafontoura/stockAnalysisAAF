#ifndef GRAPHICHANDLER_H
#define GRAPHICHANDLER_H

#include "qcustomplot.h"

class GraphicHandler : public QObject
{
    Q_OBJECT

public:
    GraphicHandler();
    QCustomPlot *getPlotHandler();
private:
    QCustomPlot *gHandler;

public slots:
    void zoomChanged();

signals:
    void zoomHasChanged();


};

#endif // GRAPHICHANDLER_H
