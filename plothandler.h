#ifndef PLOTHANDLER_H
#define PLOTHANDLER_H

#include "qcustomplot.h"


class PlotHandler : public QObject
{
    Q_OBJECT

public:
    PlotHandler(QCustomPlot *plotter);
    void setPlotter(QCustomPlot *plotter);

public slots:
    void zoomChanged();

signals:
    void zoomHasChanged();

private:

    QCustomPlot *mainPlot;
};

#endif // PLOTHANDLER_H
