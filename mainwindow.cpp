#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setupUi();




    newAverage9 = new AverageAAF(9);
    newAverage21 = new AverageAAF(21);
    newAnalysis = new dataCrossAnalysis((dataInterface<qreal>*) newAverage9,
                                        (dataInterface<qreal>*) newAverage21);

    connect(&st,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(handleDownloadProgress(qint64,qint64)));
    connect(&st,SIGNAL(downloadDone()),this,SLOT(newData()));




}

void MainWindow::handleDownloadProgress(qint64 a, qint64 b)
{

    qDebug() << "Progress - " << a << " - " << b;

}

void MainWindow::newData()
{
    QVector<qreal> x,original,rawClose,t;
    QDateTime start = QDateTime(QDate(2014, 6, 11));
    start.setTimeSpec(Qt::UTC);
    double startTime = st.data(0,(int)StockModel::DateRole).toDateTime().toTime_t();
    double day = 24;    
    double binSize = 60*60*day; // bin data in 1 day intervals
    mainPlot->clearPlottables();
    mainPlot->setBackground(Qt::black);
    mainPlot->addGraph();
    mainPlot->addGraph();
    mainPlot->addGraph();
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(1);
    pen.setColor(Qt::yellow);
    mainPlot->graph(1)->setPen(pen);
    mainPlot->graph(1)->setPen(QPen(QColor(Qt::red)));
    //mainPlot->graph(1)->setBrush(QBrush(QColor(255,50,30,20)));

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

    QCPFinancial *candlesticks = new QCPFinancial(mainPlot->xAxis, mainPlot->yAxis);
    mainPlot->addPlottable(candlesticks);
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


    mainPlot->graph(0)->setData(newAverage9->getTimeVector(),newAverage9->getData());
    mainPlot->graph(1)->setData(newAverage21->getTimeVector(),newAverage21->getData());
    mainPlot->graph(2)->setData(timeAdjusted,adjustedPrice);
    //mainPlot->graph(2)->setData(x,original);
    mainPlot->graph(0)->rescaleAxes(true);
    mainPlot->graph(1)->rescaleAxes(true);
   // mainPlot->graph(2)->rescaleAxes(true);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    mainPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);




    // configure bottom axis to show date and time instead of number:
    mainPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    mainPlot->xAxis->setDateTimeFormat("MMMM-dd\nyy");
    // set a more compact font size for bottom and left axis tick labels:
    mainPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    mainPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set a fixed tick-step to one tick per month:
    mainPlot->xAxis->setAutoTickStep(true);
    //mainPlot->xAxis->setTickStep(2628000); // one month in seconds
    mainPlot->xAxis->setSubTickCount(3);
    mainPlot->xAxis->setTickPen(QPen(QColor(Qt::white)));
    mainPlot->yAxis->setTickPen(QPen(QColor(Qt::white)));
    mainPlot->xAxis->setTickLabelColor(QColor(Qt::white));
    mainPlot->xAxis->setBasePen(QPen(QColor(Qt::white)));
    //mainPlot->set
    mainPlot->yAxis->setTickLabelColor(QColor(Qt::white));

    // apply manual tick and tick label for left axis:
    mainPlot->yAxis->setAutoTicks(false);
    mainPlot->yAxis->setAutoTickLabels(false);
    mainPlot->yAxis->setTickVector(QVector<double>() << 5.0 << 10.0 << 15.0 << 20.0);
    //mainPlot->yAxis->setTickVectorLabels(QVector<QString>() << "Not so\nhigh" << "Very\nhigh");
    // set axis labels:
    mainPlot->xAxis->setLabel("Date");
    mainPlot->yAxis->setLabel("Random wobbly lines value");
    // make top and right axes visible but without ticks and labels:
    mainPlot->xAxis2->setVisible(true);
    mainPlot->yAxis2->setVisible(true);
    mainPlot->xAxis2->setTicks(false);
    mainPlot->yAxis2->setTicks(false);
    mainPlot->xAxis2->setTickLabels(false);
    mainPlot->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    //mainPlot->xAxis->setRange(now, now+24*3600*249);
    mainPlot->yAxis->setRange(0, 60);
    // show legend:
    mainPlot->legend->setVisible(true);



    //mainPlot->xAxis->setBasePen(Qt::NoPen);
    //  mainPlot->xAxis->setTickLabels(false);
    //  mainPlot->xAxis->setTicks(false); // only want vertical grid in main axis rect, so hide xAxis backbone, ticks, and labels
    //  mainPlot->xAxis->setAutoTickStep(false);
    //  mainPlot->xAxis->setTickStep(3600*24); // 4 day tickstep
      mainPlot->rescaleAxes();
      //mainPlot->xAxis->scaleRange(1.025, mainPlot->xAxis->range().center());
      //mainPlot->yAxis->scaleRange(1.1, mainPlot->yAxis->range().center());




   /*

   // add label for phase tracer:
   QCPItemText *phaseTracerText = new QCPItemText(mainPlot);
   mainPlot->addItem(phaseTracerText);
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
   /*QCPItemText *textLabel = new QCPItemText(mainPlot);
   mainPlot->addItem(textLabel);
   textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
   textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
   textLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
   textLabel->setText("Text Item Demo");
   textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
   textLabel->setPen(QPen(Qt::white)); // show black border around text

   // add the arrow:
   QCPItemLine *arrow = new QCPItemLine(mainPlot);
   mainPlot->addItem(arrow);
   arrow->start->setParentAnchor(textLabel->bottom);
   arrow->end->setCoords(0, 0); // point to (4, 1.6) in x-y-plot coordinates
   arrow->setHead(QCPLineEnding::esSpikeArrow);
   arrow->setPen(QPen(Qt::white)); // show black border around text

   QCPItemLine *arrow2 = new QCPItemLine(mainPlot);
   mainPlot->addItem(arrow2);
   arrow2->start->setParentAnchor(textLabel->bottom);
   arrow2->end->setCoords(40, 60); // point to (4, 1.6) in x-y-plot coordinates
   arrow2->setHead(QCPLineEnding::esSpikeArrow);
   arrow2->end->setType(QCPItemPosition::ptPlotCoords);
   arrow2->setPen(QPen(Qt::white)); // show black border around text*/





    newAnalysis->analyseStock();
    BalanceAnalysis* gainAnalysis = new BalanceAnalysis(newAnalysis->getTransactionOrderList(),&st);
    gainAnalysis->analyseBalance(QDate(2008,4,21)/*QDate()*/,QDate::currentDate());
    gainAnalysis->analyseBalance(QDate(),QDate(2014,12,21));///**/,QDate::currentDate());



    if (newAnalysis->getTransactionOrderList().size() > 0)
    {
        QCPItemEllipse *buySellPosition = new QCPItemEllipse(mainPlot);
        mainPlot->addItem(buySellPosition);
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

         buySellPosition->setVisible(true);
    }

    //buySellPosition->setBrush(QBrush(QColor(Qt::)));

    //buySellPosition->set


    mainPlot->rescaleAxes();
    mainPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   //mainGridLayout->children()[0]->

}*/

void MainWindow::setupUi()
{

    this->resize(700,600);

    //actionOpen_Stock = new QAction(this);
    mainWidget = new QWidget();
    mainPlot = new QCustomPlot();
    //mainPlot->sizePolicy().setHorizontalStretch(5);
    this->setCentralWidget(mainWidget);


    tabWidget = new QTabWidget();
    tab = new QWidget();

    tabWidget->addTab(tab,QString("Info"));
    //tabWidget->sizePolicy().setHorizontalStretch(1);


    mainGridLayout = new QGridLayout();
    mainWidget->setLayout(mainGridLayout);


    mainGridLayout->addWidget(tabWidget,0,0);
    mainGridLayout->addWidget(mainPlot,0,1);

    mainGridLayout->setColumnStretch(0,1);
    mainGridLayout->setColumnStretch(1,5);
    //mainGridLayout->



/*

        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(769, 506);
        actionOpen_Stock = new QAction(MainWindow);
        actionOpen_Stock->setObjectName(QStringLiteral("actionOpen_Stock"));
        mainWidget = new QWidget(MainWindow);
        mainWidget->setObjectName(QStringLiteral("mainWidget"));
        customPlot = new QCustomPlot(mainWidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        customPlot->setGeometry(QRect(189, 19, 561, 431));
        tabWidget = new QTabWidget(mainWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(6, 19, 171, 431));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(mainWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 769, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_Stock);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);


        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionOpen_Stock->setText(QApplication::translate("MainWindow", "Open Stock", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));*/


}

void MainWindow::on_actionOpen_Stock_triggered()
{
    qDebug() << "clicked";


    st.setDataBaseType(StockModel::LocalCSV);

    st.setFileName(QFileDialog::getOpenFileName());
    st.requestData();
}

void MainWindow::on_actionRequest_Web_Stock_triggered()
{
    st.setStockName("RLOG3.SA");
    st.setStartDate(QDate(2012,11,10));
    //st.setEndDate(QDate(2012,11,30));

    st.setDataBaseType(StockModel::WebCSV);
    st.requestData();
}
