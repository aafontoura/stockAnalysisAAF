#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stockprice.h"
#include "averageaaf.h"
#include "datacrossanalysis.h"
#include "datainterface.h"
#include "balanceanalysis.h"
#include "qcustomplot.h" // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.
#include "stocksHandler/sitem.h"
#include "graphichandler.h"
#include "smodelview.h"
#include "stocksHandler/sitemtreemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QAction *actionOpen_Stock;
    QWidget *mainWidget;
    //QCustomPlot *customPlot;
    QTabWidget *tabWidget;
    QWidget *tab;
    /*QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;*/
    QGridLayout *mainGridLayout;
    QCustomPlot *mainPlot;
    GraphicHandler *mainGraph;
    SModelView *stockModelView;


    SItemTreeModel *nModel;
    QItemSelectionModel *customSelectionModel;

    StockModel st;
    StockPrice sp;
    AverageAAF* newAverage9;
    AverageAAF* newAverage21;
    dataCrossAnalysis *newAnalysis;
//StockModel newStock;

    void setupUi();

private slots:
    void handleDownloadProgress(qint64 a,qint64 b);
    void newData(void);
    void updateGraph(const QModelIndex &index);
    void stocksSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void currentSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_actionOpen_Stock_triggered();
    void on_actionRequest_Web_Stock_triggered();
    void on_actionAverage_triggered();
};

#endif // MAINWINDOW_H

