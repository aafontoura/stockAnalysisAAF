#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QDebug>
#include "stocksHandler/stockholder.h"
#include "stocksHandler/sitemtreemodel.h"
#include "stocksHandler/indicatorholder.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setupUi();




    newAverage9 = new AverageAAF(9);
    newAverage21 = new AverageAAF(21);
    newAnalysis = new dataCrossAnalysis((dataInterface*) newAverage9,
                                        (dataInterface*) newAverage21);

    connect(&st,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(handleDownloadProgress(qint64,qint64)));
    connect(&st,SIGNAL(downloadDone()),this,SLOT(newData()));




}

void MainWindow::handleDownloadProgress(qint64 a, qint64 b)
{

    qDebug() << "Progress - " << a << " - " << b;

}

void MainWindow::newData()
{
    //if ()
    //this->stockModelView->setStockData(Stocks->child(0)->getData());
}

void MainWindow::updateGraph(const QModelIndex &index)
{
    this->stockModelView->setStockData(nModel->getItem(index));
}

void MainWindow::stocksSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{


}

void MainWindow::currentSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{

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
    stockModelView = new SModelView(new GraphicHandler());

    /*mainGraph = new GraphicHandler();*/
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
    mainGridLayout->addWidget(stockModelView->getGraphDrawer(),0,1);

    mainGridLayout->setColumnStretch(0,1);
    mainGridLayout->setColumnStretch(1,5);
    //mainGridLayout->

    nModel = new SItemTreeModel(new SItem());
    customSelectionModel = new QItemSelectionModel(nModel);


    QTreeView *viewTree;
    viewTree = new QTreeView(tab);
    viewTree->setModel(nModel);

    viewTree->setSelectionModel(customSelectionModel);

    //connect(customSelectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(stocksSelectionChanged(QItemSelection,QItemSelection)));
    //connect(customSelectionModel,SIGNAL(currentChanged(QModelIndex,QModelIndex)),this,SLOT(currentSelectionChanged(QModelIndex,QModelIndex)));


    viewTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //connect(viewtree,SIGNAL())
    connect(viewTree,SIGNAL(clicked(QModelIndex)),this,SLOT(updateGraph(QModelIndex)));
}

void MainWindow::on_actionOpen_Stock_triggered()
{
    qDebug() << "clicked";

    StockModel *newStock = new StockModel();

    connect(newStock,SIGNAL(downloadDone()),this,SLOT(newData()));


    newStock->setDataBaseType(StockModel::LocalCSV);
    newStock->setFileName(QFileDialog::getOpenFileName());

    nModel->addItem((SItemData*)new StockHolder(newStock));
    newStock->requestData();
}

void MainWindow::on_actionRequest_Web_Stock_triggered()
{
    StockModel *newStock = new StockModel();

    connect(newStock,SIGNAL(downloadDone()),this,SLOT(newData()));

    newStock->setStockName("RLOG3.SA");
    newStock->setDataBaseType(StockModel::WebCSV);
    newStock->requestData();

    nModel->addItem((SItemData*)new StockHolder(newStock));



}

void MainWindow::on_actionAverage_triggered()
{
    dataInterface *newAverage = new AverageAAF(9);

    QModelIndexList list = customSelectionModel->selectedIndexes();

    QModelIndexList stocksIdx;
    foreach (QModelIndex idx, list)
    {
        QModelIndex newIdx = idx;

        if (SItemData::Stock != static_cast<SItem*>(idx.internalPointer())->getData()->getType())
        {
            QModelIndex temp = idx.parent();
            newIdx = temp;
        }

        if (!stocksIdx.contains(newIdx))
            stocksIdx.append(newIdx);
    }

    foreach (QModelIndex idx, stocksIdx)
    {
        nModel->addItem(static_cast<SItemData*>((new IndicatorHolder(newAverage))),idx);
    }
    //delete(newIndicator);

}
