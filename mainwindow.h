#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stockprice.h"
#include "averageaaf.h"
#include "datacrossanalysis.h"
#include "datainterface.h"
#include "balanceanalysis.h"
#include "qcustomplot.h" // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.

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
    StockModel st;
    StockPrice sp;
    AverageAAF* newAverage9;
    AverageAAF* newAverage21;
    dataCrossAnalysis *newAnalysis;

private slots:
    void handleDownloadProgress(qint64 a,qint64 b);
    void newData(void);

};

#endif // MAINWINDOW_H

