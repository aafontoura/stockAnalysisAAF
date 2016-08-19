#ifndef DATAINTERFACE_H
#define DATAINTERFACE_H

#include <QCoreApplication>

template <class T>
class dataInterface
{
public:


    virtual qreal getValueIndex(qint32 index) = 0;
    virtual qreal getValueTime(quint32 time) = 0;
    virtual qint32 getDelayOfsset(void) = 0;
    virtual qint32 getLength(void) = 0;
    virtual QVector<double> getTimeVector() = 0;
    virtual QVector<T> getData() = 0;
};

#endif // DATAINTERFACE_H
