#-------------------------------------------------
#
# Project created by QtCreator 2016-05-22T23:18:07
#
#-------------------------------------------------

QT       += core gui network
CONFIG += stl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = TestPlot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    averageaaf.cpp \
    datacrossanalysis.cpp \
    datainterface.cpp \
    stockprice.cpp \
    transactionorder.cpp \
    filedownload.cpp \
    qcustomplot.cpp \
    stockanalysis.cpp \
    balanceanalysis.cpp \
    plothandler.cpp

HEADERS  += mainwindow.h \
    AAFAnalysisDefs.h \
    averageaaf.h \
    datacrossanalysis.h \
    datainterface.h \
    stockprice.h \
    transactionorder.h \
    filedownload.h \
    qcustomplot.h \
    stockanalysis.h \
    balanceanalysis.h \
    plothandler.h

FORMS    += mainwindow.ui
