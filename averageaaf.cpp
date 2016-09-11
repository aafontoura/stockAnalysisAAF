#include "averageaaf.h"


AverageAAF::AverageAAF()
{
    values.clear();
    time.clear();
    this->averageSize = 6;
    buffer.resize(6);

    indexBuffer = 0;
    sum = 0;
}

AverageAAF::AverageAAF(qint32 size)
{
    values.clear();
    time.clear();
    this->averageSize = size;
    buffer.resize(size);

    indexBuffer = 0;
    sum = 0;

}

double AverageAAF::getValueIndex(qint32 index)
{
    return getAverageIndex(index);
}

double AverageAAF::getValueTime(quint32 time)
{
    return getAverageTime(time);
}

qint32 AverageAAF::getDelayOfsset(void)
{
    return this->averageSize;
}

qint32 AverageAAF::getLength(void)
{
    return values.size();
}

QVector<double> AverageAAF::getData()
{
    /* Exludes the first elements. For the Average perspective they are not relevant */
    return values.mid(averageSize-1,values.size()-averageSize);
}

void AverageAAF::addItem(qreal item, quint32 timeIn)
{
    insertNewValue(item,timeIn);
}

void AverageAAF::addItem(qreal item)
{
    insertNewValue(item);

}

QVector<double> AverageAAF::getTimeVector()
{
    /* Exludes the first elements. For the Average perspective they are not relevant */
    return time.mid(averageSize-1,values.size()-averageSize);
}

QVector<double> AverageAAF::getTimeVectorAdjusted(unsigned int delta)
{
    QVector<double> timeVector;
    //timeVector.resize(time.count());

    int init = time.first();
    for(int i = 0;i<time.count();i++)
    {
        timeVector.append(init+(double)i*(double)delta);
    }

    /* Exludes the first elements. For the Average perspective they are not relevant */
    return timeVector.mid(averageSize-1,values.size()-averageSize);
}




void AverageAAF::insertNewValue(double newValue)
{
    sum -= buffer[indexBuffer];
    buffer[indexBuffer] = newValue;
    sum += newValue;
    indexBuffer = ++indexBuffer % averageSize;


    values.push_back(sum/averageSize);
    time << time.size();

}

void AverageAAF::insertNewValue(double newValue, quint32 timeIn)
{
    sum -= buffer[indexBuffer];
    buffer[indexBuffer] = newValue;
    sum += newValue;
    indexBuffer = ++indexBuffer % averageSize;


    time << timeIn;

    /* Insert the value in the correct place */
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



void AverageAAF::clear()
{
    values.clear();
    time.clear();

    indexBuffer = 0;
    sum = 0;
}

double AverageAAF::getAverageIndex(qint32 index)
{
    if (index<values.size())
        return values[index];
    else
        return 0;
}
double AverageAAF::getAverageTime(quint32 timeIn)
{
    int i;
    for (i=0;i<values.size() && time.at(i) < timeIn;i++);

    return values.at(i);
}


