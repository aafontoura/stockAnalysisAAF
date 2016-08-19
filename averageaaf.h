#ifndef AVERAGEAAF_H
#define AVERAGEAAF_H

#include "datainterface.h"
#include <QList>
#include <QVector>
#include <algorithm>

class AverageAAF : dataInterface<qreal>
{


public:
    AverageAAF();
    AverageAAF(qint32 size);

    void insertNewValue(qreal newValue);
    void insertNewValue(qreal newValue, quint32 timeIn);
    qreal getAverageIndex(qint32 index);
    qreal getAverageTime(quint32 timeIn);
    qreal getValueIndex(qint32 index);
    qreal getValueTime(quint32 time);
    qint32 getDelayOfsset(void);
    qint32 getLength(void);
    QVector<qreal> getData();
    QVector<double> getTimeVector();


private:
    QVector <qreal> values;
    qint32 averageSize;
    QVector<qreal> buffer;
    qreal sum;
    qint32 indexBuffer;
    QVector<double> time;


};

#endif // AVERAGEAAF_H
