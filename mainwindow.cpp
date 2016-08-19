#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    newAverage9 = new AverageAAF(9);
    newAverage21 = new AverageAAF(21);
    newAnalysis = new dataCrossAnalysis((dataInterface<qreal>*) newAverage9,
                                        (dataInterface<qreal>*) newAverage21);

    connect(&st,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(handleDownloadProgress(qint64,qint64)));
    connect(&st,SIGNAL(downloadDone()),this,SLOT(newData()));

    st.setStockName("RLOG3.SA");
    st.setStartDate(QDate(2012,11,10));
    //st.setEndDate(QDate(2012,11,30));
    st.requestData();

}

void MainWindow::handleDownloadProgress(qint64 a, qint64 b)
{

    //qDebug() << a << " - " << b;

}

void MainWindow::newData()
{
    QVector<qreal> x,original,rawClose,t;
    QDateTime start = QDateTime(QDate(2014, 6, 11));
    start.setTimeSpec(Qt::UTC);
    double startTime = st.data(0,(int)StockModel::DateRole).toDateTime().toTime_t();
    double day = 24;    
    double binSize = 60*60*day; // bin data in 1 day intervals
    ui->customPlot->setBackground(Qt::black);
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    pen.setColor(Qt::yellow);
    ui->customPlot->graph(1)->setPen(pen);
    ui->customPlot->graph(1)->setPen(QPen(QColor(Qt::red)));
    //ui->customPlot->graph(1)->setBrush(QBrush(QColor(255,50,30,20)));

    QVector<double> adjustedPrice,timeAdjusted;

    for (int i = 0; i<st.rowCount()-1;i++)
    {
        double currentTime = st.data(i,(int)StockModel::DateRole).toDateTime().toTime_t();

        //x.append(i*binSize);
        x.append(currentTime);
        original.append(st.data(i,(int)StockModel::OpenPriceRole).toFloat());
        x.append(x[x.size()-1]+6*60);
        original.append(st.data(i,(int)StockModel::LowPriceRole).toFloat());
        x.append(x[x.size()-1]+6*60);
        original.append(st.data(i,(int)StockModel::HighPriceRole).toFloat());
        x.append(x[x.size()-1]+6*60);
        original.append(st.data(i,(int)StockModel::ClosePriceRole).toFloat());
        rawClose.append(st.data(i,(int)StockModel::ClosePriceRole).toFloat());

        newAverage9->insertNewValue(st.data(i,(int)StockModel::AdjustedPriceRole).toFloat(),currentTime);
        newAverage21->insertNewValue(st.data(i,(int)StockModel::AdjustedPriceRole).toFloat(),currentTime);

        adjustedPrice.append(st.data(i,(int)StockModel::AdjustedPriceRole).toDouble());
        timeAdjusted.append(currentTime);
    }

    QCPFinancial *candlesticks = new QCPFinancial(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(candlesticks);
    QCPFinancialDataMap data1 = QCPFinancial::timeSeriesToOhlc(x, original, binSize,startTime);//24*60);
    candlesticks->setName("Candlestick");
    candlesticks->setChartStyle(QCPFinancial::csCandlestick);
    candlesticks->setData(&data1, true);
    candlesticks->setWidth(binSize);
    candlesticks->setTwoColored(true);
    candlesticks->setBrushPositive(QColor(0, 245, 0));
    candlesticks->setBrushNegative(QColor(245, 0, 0));
    candlesticks->setPenPositive(QPen(Qt::white));
    candlesticks->setPenNegative(QPen(Qt::white));


    ui->customPlot->graph(0)->setData(newAverage9->getTimeVector(),newAverage9->getData());
    ui->customPlot->graph(1)->setData(newAverage21->getTimeVector(),newAverage21->getData());
    ui->customPlot->graph(2)->setData(timeAdjusted,adjustedPrice);
    //ui->customPlot->graph(2)->setData(x,original);
    ui->customPlot->graph(0)->rescaleAxes(true);
    ui->customPlot->graph(1)->rescaleAxes(true);
   // ui->customPlot->graph(2)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);




    // configure bottom axis to show date and time instead of number:
    ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->customPlot->xAxis->setDateTimeFormat("MMMM-dd\nyy");
    // set a more compact font size for bottom and left axis tick labels:
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set a fixed tick-step to one tick per month:
    ui->customPlot->xAxis->setAutoTickStep(true);
    //ui->customPlot->xAxis->setTickStep(2628000); // one month in seconds
    ui->customPlot->xAxis->setSubTickCount(3);
    ui->customPlot->xAxis->setTickPen(QPen(QColor(Qt::white)));
    ui->customPlot->yAxis->setTickPen(QPen(QColor(Qt::white)));
    ui->customPlot->xAxis->setTickLabelColor(QColor(Qt::white));
    ui->customPlot->xAxis->setBasePen(QPen(QColor(Qt::white)));
    //ui->customPlot->set
    ui->customPlot->yAxis->setTickLabelColor(QColor(Qt::white));

    // apply manual tick and tick label for left axis:
    ui->customPlot->yAxis->setAutoTicks(false);
    ui->customPlot->yAxis->setAutoTickLabels(false);
    ui->customPlot->yAxis->setTickVector(QVector<double>() << 5.0 << 10.0 << 15.0 << 20.0);
    //ui->customPlot->yAxis->setTickVectorLabels(QVector<QString>() << "Not so\nhigh" << "Very\nhigh");
    // set axis labels:
    ui->customPlot->xAxis->setLabel("Date");
    ui->customPlot->yAxis->setLabel("Random wobbly lines value");
    // make top and right axes visible but without ticks and labels:
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTicks(false);
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    //ui->customPlot->xAxis->setRange(now, now+24*3600*249);
    ui->customPlot->yAxis->setRange(0, 60);
    // show legend:
    ui->customPlot->legend->setVisible(true);



    //ui->customPlot->xAxis->setBasePen(Qt::NoPen);
    //  ui->customPlot->xAxis->setTickLabels(false);
    //  ui->customPlot->xAxis->setTicks(false); // only want vertical grid in main axis rect, so hide xAxis backbone, ticks, and labels
    //  ui->customPlot->xAxis->setAutoTickStep(false);
    //  ui->customPlot->xAxis->setTickStep(3600*24); // 4 day tickstep
      ui->customPlot->rescaleAxes();
      //ui->customPlot->xAxis->scaleRange(1.025, ui->customPlot->xAxis->range().center());
      //ui->customPlot->yAxis->scaleRange(1.1, ui->customPlot->yAxis->range().center());




   /*

   // add label for phase tracer:
   QCPItemText *phaseTracerText = new QCPItemText(ui->customPlot);
   ui->customPlot->addItem(phaseTracerText);
   phaseTracerText->position->setType(QCPItemPosition::ptAxisRectRatio);
   phaseTracerText->setPositionAlignment(Qt::AlignRight|Qt::AlignBottom);
   phaseTracerText->position->setCoords(1.0, 0.95); // lower right corner of axis rect
   phaseTracerText->setText("Points of fixed\nphase define\nphase velocity vp");
   phaseTracerText->setTextAlignment(Qt::AlignLeft);
   phaseTracerText->setFont(QFont(font().family(), 9));
   phaseTracerText->setPadding(QMargins(8, 0, 0, 0));
   phaseTracerText->setPen(QPen(QColor(Qt::white)));
   phaseTracerText->setColor(QColor(Qt::white));*/

   // add the text label at the top:
   /*QCPItemText *textLabel = new QCPItemText(ui->customPlot);
   ui->customPlot->addItem(textLabel);
   textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
   textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
   textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
   textLabel->setText("Text Item Demo");
   textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
   textLabel->setPen(QPen(Qt::white)); // show black border around text

   // add the arrow:
   QCPItemLine *arrow = new QCPItemLine(ui->customPlot);
   ui->customPlot->addItem(arrow);
   arrow->start->setParentAnchor(textLabel->bottom);
   arrow->end->setCoords(0, 0); // point to (4, 1.6) in x-y-plot coordinates
   arrow->setHead(QCPLineEnding::esSpikeArrow);
   arrow->setPen(QPen(Qt::white)); // show black border around text

   QCPItemLine *arrow2 = new QCPItemLine(ui->customPlot);
   ui->customPlot->addItem(arrow2);
   arrow2->start->setParentAnchor(textLabel->bottom);
   arrow2->end->setCoords(40, 60); // point to (4, 1.6) in x-y-plot coordinates
   arrow2->setHead(QCPLineEnding::esSpikeArrow);
   arrow2->end->setType(QCPItemPosition::ptPlotCoords);
   arrow2->setPen(QPen(Qt::white)); // show black border around text*/





    newAnalysis->analyseStock();
    BalanceAnalysis* gainAnalysis = new BalanceAnalysis(newAnalysis->getTransactionOrderList(),&st);
    gainAnalysis->analyseBalance(QDate(2008,4,21)/*QDate()*/,QDate::currentDate());
    gainAnalysis->analyseBalance(QDate(),QDate(2014,12,21));///**/,QDate::currentDate());



    QCPItemEllipse *buySellPosition = new QCPItemEllipse(ui->customPlot);
    ui->customPlot->addItem(buySellPosition);
    buySellPosition->topLeft->setType(QCPItemPosition::ptPlotCoords);
    buySellPosition->topLeft->setCoords(newAnalysis->getLatestTransactionOrder().getTime_t()-24*60*60,newAverage9->getValueTime(newAnalysis->getLatestTransactionOrder().getTime_t())+0.5);
    buySellPosition->bottomRight->setType(QCPItemPosition::ptPlotCoords);
    buySellPosition->bottomRight->setCoords(newAnalysis->getLatestTransactionOrder().getTime_t()+24*60*60,newAverage9->getValueTime(newAnalysis->getLatestTransactionOrder().getTime_t())-0.5);
    if (BOUGHT_STOCK == newAnalysis->getLatestTransactionOrder().getStatus())
    {
        buySellPosition->setPen(QPen(QColor(Qt::green)));
    }
    else
    {
        buySellPosition->setPen(QPen(QColor(Qt::red)));
    }

    //buySellPosition->setBrush(QBrush(QColor(Qt::)));
    buySellPosition->setVisible(true);
    //buySellPosition->set


    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
