#ifndef AVERAGEAAF_H
#define AVERAGEAAF_H

#include "datainterface.h"
#include <QList>
#include <QVector>
#include <algorithm>

class AverageAAF : dataInterface
{


public:
    AverageAAF();
    AverageAAF(qint32 size);

    void insertNewValue(double newValue);
    void insertNewValue(double newValue, quint32 timeIn);
    double getAverageIndex(qint32 index);
    double getAverageTime(quint32 timeIn);
    double getValueIndex(qint32 index);
    double getValueTime(quint32 time);
    qint32 getDelayOfsset(void);
    qint32 getLength(void);
    QVector<double> getData();
    QVector<double> getTimeVector();
    void clear();


private:
    QVector <qreal> values;
    qint32 averageSize;
    QVector<qreal> buffer;
    qreal sum;
    qint32 indexBuffer;
    QVector<double> time;


};

#endif // AVERAGEAAF_H
