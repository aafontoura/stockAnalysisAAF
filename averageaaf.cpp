#include "averageaaf.h"


AverageAAF::AverageAAF()
{
    values.clear();
    this->averageSize = 6;
    buffer.resize(6);

    indexBuffer = 0;
    sum = 0;
}

AverageAAF::AverageAAF(qint32 size)
{
    values.clear();
    this->averageSize = size;
    buffer.resize(size);

    indexBuffer = 0;
    sum = 0;

}

void AverageAAF::insertNewValue(qreal newValue)
{
    sum -= buffer[indexBuffer];
    buffer[indexBuffer] = newValue;
    sum += newValue;
    indexBuffer = ++indexBuffer % averageSize;


    values.push_back(sum/averageSize);
    time << time.size();

}

void AverageAAF::insertNewValue(qreal newValue, quint32 timeIn)
{
    sum -= buffer[indexBuffer];
    buffer[indexBuffer] = newValue;
    sum += newValue;
    indexBuffer = ++indexBuffer % averageSize;


    time << timeIn;
    if ((time.size() > 1) && (timeIn < time[time.size()-2]))
    {
        std::sort(time.begin(),time.end());
        values.insert(time.indexOf(timeIn),sum/averageSize);
    }
    else
    {
        values << sum/averageSize;
    }
}

qint32 AverageAAF::getLength(void)
{
    return values.size();
}

QVector<qreal> AverageAAF::getData()
{
    /* Exludes the first elements. For the Average perspective they are not relevant */
    return values.mid(averageSize-1,values.size()-averageSize);
}

QVector<double> AverageAAF::getTimeVector()
{
    /* Exludes the first elements. For the Average perspective they are not relevant */
    return time.mid(averageSize-1,values.size()-averageSize);
}

qreal AverageAAF::getAverageIndex(qint32 index)
{
    if (index<values.size())
        return values[index];
    else
        return 0;
}
qreal AverageAAF::getAverageTime(quint32 timeIn)
{
    int i;
    for (i=0;i<values.size() && time.at(i) < timeIn;i++);

    return values.at(i);
}

qreal AverageAAF::getValueTime(quint32 time)
{
    return getAverageTime(time);
}

qreal AverageAAF::getValueIndex(qint32 index)
{
    return getAverageIndex(index);
}

qint32 AverageAAF::getDelayOfsset(void)
{
    return this->averageSize;
}
