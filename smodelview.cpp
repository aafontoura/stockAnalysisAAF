#include "smodelview.h"
// Deletar linha
#include "stocksHandler/indicatorholder.h"



SModelView::SModelView()
{

}

SModelView::SModelView(GraphicHandler *plotHandler, SItemData *displayData)
{
    this->plotHandler = plotHandler;


    if ((NULL != displayData) && (SItemData::Stock == displayData->getType()))
    {
        /* TBD */
        this->displayData = static_cast<StockHolder*>(displayData);
    }

    connect (this->plotHandler,SIGNAL(zoomHasChanged()),this,SLOT(zoomChanged()));

}

void SModelView::setStockData(SItem *sData)
{
    if (SItemData::Stock == sData->getData()->getType())
    {
        this->displayData = static_cast<StockHolder*>(sData->getData());
        data.clear();

        for (int i = 0 ; i< sData->childCount();i++)
            data.append(static_cast<IndicatorHolder*>(sData->child(i)->getData())->getData());

        if (NULL != plotHandler)
        {
            StockModel *currentModel = displayData->getSTTemp();
            QVector<qreal> x,original;

            axisVector.clear();
            axisVectorLabel.clear();


            /* General config */
            double startTime = currentModel->data(0,(int)StockModel::DateRole).toDateTime().toTime_t();
            //double day = 24;
            double binSize = C_ONE_DAY_T; // bin data in 1 day intervals

            /* Clear previous graph */
            plotHandler->getPlotHandler()->clearPlottables();

            /* Graph look settings */
            plotHandler->getPlotHandler()->setBackground(Qt::black);
            QPen pen;
            pen.setStyle(Qt::DashLine);
            pen.setWidth(1);
            pen.setColor(Qt::yellow);

            /* add additional graphs */
            for(int i = 0 ; i < data.count(); i++)
            {
                plotHandler->getPlotHandler()->addGraph();
                plotHandler->getPlotHandler()->graph(i)->setData(data[i]->getTimeVectorAdjusted(binSize),data[i]->getData());
                plotHandler->getPlotHandler()->graph(i)->setName(sData->child(i)->getData()->getName());
            }


            for (int i = 0; i<currentModel->rowCount()-1;i++)
            {
                //double currentTime = currentModel->data(i,(int)StockModel::DateRole).toDateTime().toTime_t();
                double currentTime = startTime + binSize*i;

                //x.append(i*binSize);
                x.append(currentTime);
                original.append(currentModel->data(i,(int)StockModel::OpenPriceRole).toFloat());
                x.append(x[x.size()-1]+6*60);
                original.append(currentModel->data(i,(int)StockModel::LowPriceRole).toFloat());
                x.append(x[x.size()-1]+6*60);
                original.append(currentModel->data(i,(int)StockModel::HighPriceRole).toFloat());
                x.append(x[x.size()-1]+6*60);
                original.append(currentModel->data(i,(int)StockModel::ClosePriceRole).toFloat());



                    axisVector.append(currentTime);
                    QDateTime dt;
                    dt.setTime_t(currentModel->data(i,(int)StockModel::DateRole).toDateTime().toTime_t());
                    axisVectorLabel.append(dt.toString("MMM dd\n yy"));
                   // axisVectorLabel.append("str - "+i);

            }



            QCPFinancial *candlesticks = new QCPFinancial(plotHandler->getPlotHandler()->xAxis, plotHandler->getPlotHandler()->yAxis);

            plotHandler->getPlotHandler()->addPlottable(candlesticks);
            QCPFinancialDataMap data1 = QCPFinancial::timeSeriesToOhlc(x, original, binSize,startTime);//24*60);
            candlesticks->setName(currentModel->stockName());
            candlesticks->setChartStyle(QCPFinancial::csCandlestick);
            candlesticks->setData(&data1, true);
            candlesticks->setWidth(binSize*0.9);
            candlesticks->setTwoColored(true);
            candlesticks->setBrushPositive(QColor(0, 245, 0));
            candlesticks->setBrushNegative(QColor(245, 0, 0));
            candlesticks->setPenPositive(QPen(Qt::white));
            candlesticks->setPenNegative(QPen(Qt::white));



            plotHandler->getPlotHandler()->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);




            // configure bottom axis to show date and time instead of number:
            plotHandler->getPlotHandler()->xAxis->setTickLabelType(QCPAxis::ltDateTime);
            plotHandler->getPlotHandler()->xAxis->setDateTimeFormat("MMMM-dd\nyy");
            // set a more compact font size for bottom and left axis tick labels:
            plotHandler->getPlotHandler()->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
            plotHandler->getPlotHandler()->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
            // set a fixed tick-step to one tick per month:
            plotHandler->getPlotHandler()->xAxis->setAutoTickStep(false);
            plotHandler->getPlotHandler()->xAxis->setAutoTicks(false);
            plotHandler->getPlotHandler()->xAxis->setAutoTickLabels(false);

            //plotHandler->getPlotHandler()->xAxis->setTickStep(2628000); // one month in seconds
            plotHandler->getPlotHandler()->xAxis->setSubTickCount(3);
            plotHandler->getPlotHandler()->xAxis->setTickPen(QPen(QColor(Qt::white)));
            plotHandler->getPlotHandler()->yAxis->setTickPen(QPen(QColor(Qt::white)));
            plotHandler->getPlotHandler()->xAxis->setTickLabelColor(QColor(Qt::white));
            plotHandler->getPlotHandler()->xAxis->setBasePen(QPen(QColor(Qt::white)));
            //plotHandler->getPlotHandler()->set
            plotHandler->getPlotHandler()->yAxis->setTickLabelColor(QColor(Qt::white));

            // apply manual tick and tick label for left axis:
            plotHandler->getPlotHandler()->yAxis->setAutoTicks(false);
            plotHandler->getPlotHandler()->yAxis->setAutoTickLabels(false);
            plotHandler->getPlotHandler()->yAxis->setTickVector(QVector<double>() << 5.0 << 10.0 << 15.0 << 20.0);
            //plotHandler->getPlotHandler()->yAxis->setTickVectorLabels(QVector<QString>() << "Not so\nhigh" << "Very\nhigh");
            // set axis labels:
            plotHandler->getPlotHandler()->xAxis->setLabel("Date");
            plotHandler->getPlotHandler()->yAxis->setLabel("Random wobbly lines value");
            // make top and right axes visible but without ticks and labels:
            plotHandler->getPlotHandler()->xAxis2->setVisible(true);
            plotHandler->getPlotHandler()->yAxis2->setVisible(true);
            plotHandler->getPlotHandler()->xAxis2->setTicks(false);
            plotHandler->getPlotHandler()->yAxis2->setTicks(false);
            plotHandler->getPlotHandler()->xAxis2->setTickLabels(false);
            plotHandler->getPlotHandler()->yAxis2->setTickLabels(false);
            // set axis ranges to show all data:
            //plotHandler->getPlotHandler()->xAxis->setRange(now, now+24*3600*249);
            plotHandler->getPlotHandler()->yAxis->setRange(0, 60);
            // show legend:
            plotHandler->getPlotHandler()->legend->setVisible(true);

            //plotHandler->getPlotHandler()->xAxis->setTickVector(axisVector);
            //plotHandler->getPlotHandler()->xAxis->setTickVectorLabels(axisVectorLabel);

            zoomChanged();


            plotHandler->getPlotHandler()->rescaleAxes();
            plotHandler->getPlotHandler()->replot();
        }
    }


}

StockHolder *SModelView::getStockData()
{
    return this->displayData;
}

QCustomPlot *SModelView::getGraphDrawer()
{
    return this->plotHandler->getPlotHandler();

}

void SModelView::zoomChanged()
{

    //Determine how many labels could fit on the plot(my labels are ~54 pixels wide)
    int numberOfLabels = plotHandler->getPlotHandler()->axisRect()->width() / 100;

    //Determine your viewing window
    int window = plotHandler->getPlotHandler()->xAxis->range().size()/numberOfLabels;
    QVector<double> customAxis;
    QVector<QString> customAxisLabel;

    if (plotHandler->getPlotHandler()->xAxis->range().lower < axisVector.last())
    {
        double rangeLower = plotHandler->getPlotHandler()->xAxis->range().lower;
        double rangeUpper = plotHandler->getPlotHandler()->xAxis->range().upper;

        int lower = 0;
        for (;lower<axisVector.size() && axisVector[lower] < rangeLower;lower++);

        int upper = lower;
        for (;upper<axisVector.size() && axisVector[upper] < rangeUpper;upper++);

        window = (upper-lower)/numberOfLabels;
        window = 0==window ? 1 : window;

        for (int i = lower; i < upper; i+=window)
        {
            customAxis.append(axisVector[i]);
            customAxisLabel.append(axisVectorLabel[i]);

        }

        plotHandler->getPlotHandler()->xAxis->setTickVector(customAxis);
        plotHandler->getPlotHandler()->xAxis->setTickVectorLabels(customAxisLabel);
    }



}
