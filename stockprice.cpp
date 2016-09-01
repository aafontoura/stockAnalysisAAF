#include "stockprice.h"

#include <QtCore/QUrl>
#include <QtCore/QDate>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


#include <QFileDialog>

StockModel::StockModel(QObject *parent)
    : QAbstractListModel(parent)
    , _startDate(QDate(1995, 4, 25))
    , _endDate(QDate::currentDate())
    , _dataCycle(StockModel::Daily)
    , _dataBaseType(StockModel::LocalCSV)
    , _manager(0)
    , _updating(false)
{
    QHash<int, QByteArray> roles;
    roles[StockModel::DateRole] = "date";
    roles[StockModel::SectionRole] = "year";
    roles[StockModel::OpenPriceRole] = "openPrice";
    roles[StockModel::ClosePriceRole] = "closePrice";
    roles[StockModel::HighPriceRole] = "highPrice";
    roles[StockModel::LowPriceRole] = "lowPrice";
    roles[StockModel::VolumeRole] = "volume";
    roles[StockModel::AdjustedPriceRole] = "adjustedPrice";
    //setRoleNames(roles);

    /*connect(this, SIGNAL(stockNameChanged()), SLOT(requestData()));
    connect(this, SIGNAL(startDateChanged()), SLOT(requestData()));
    connect(this, SIGNAL(endDateChanged()), SLOT(requestData()));
    connect(this, SIGNAL(dataCycleChanged()), SLOT(requestData()));*/

    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(update(QNetworkReply*)));


    /*inFile = new QFile(QFileDialog::getOpenFileName());
    inFile->open(QIODevice::ReadOnly);
    this->SetupFormatFile(inFile->readLine());*/
    this->SetupFormatFile();

}

int StockModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return _prices.count();
}

StockPrice* StockModel::stockPriceAtIndex(int idx) const
{
    if (idx >=0 && idx < _prices.size()) {
        return _prices[idx];
    }
    return 0;
}

void StockModel::requestData()
{
    switch (_dataBaseType)
    {
    case StockModel::WebCSV:
        if (!_updating)
        {
            _updating = true;
            QMetaObject::invokeMethod(this, "doRequest", Qt::QueuedConnection);
        }
        break;
    case StockModel::LocalCSV:
        inFile = new QFile(this->fileName);
        inFile->open(QIODevice::ReadOnly);
        /* checks missing!!!! */
        this->SetupFormatFile(inFile->readLine());
        this->updatePrices(inFile);
        break;
    }
}

void StockModel::doRequest()
{
    /*
        Fetch stock data from yahoo finance:
         url: http://ichart.finance.yahoo.com/table.csv?s=NOK&a=5&b=11&c=2010&d=7&e=23&f=2010&g=d&ignore=.csv
            s:stock name/id, a:start day, b:start month, c:start year  default: 25 April 1995, oldest c= 1962
            d:end day, e:end month, f:end year, default:today  (data only available 3 days before today)
            g:data cycle(d daily,  w weekly, m monthly, v Dividend)
      */
    if (_manager && !_stockName.isEmpty() && _endDate > _startDate) {
        QString query("http://ichart.finance.yahoo.com/table.csv?s=%1&a=%2&b=%3&c=%4&d=%5&e=%6&f=%7&g=%8&ignore=.csv");
        query = query.arg(_stockName)
                     .arg(_startDate.month()).arg(_startDate.day()).arg(_startDate.year())
                     .arg(_endDate.month()).arg(_endDate.day()).arg(_endDate.year())
                     .arg(dataCycleString());

        qDebug() << "request stock data:" << query;
        QNetworkReply* reply = _manager->get(QNetworkRequest(QUrl(query)));
        connect(reply, SIGNAL(downloadProgress(qint64,qint64)), SIGNAL(downloadProgress(qint64,qint64)));
    }
}

void StockModel::update(QNetworkReply *reply)
{
    _updating = false;

    if (reply)
    {
         if (reply->error() == QNetworkReply::NoError)
         {
            /*beginResetModel();

            foreach (StockPrice* p, _prices)
            {
                p->deleteLater();
            }
            _prices.clear();

            while (!reply->atEnd())
            {
                QString line = reply->readLine();
                QStringList fields = line.split(',');

                //data format:Date,Open,High,Low,Close,Volume,Adjusted close price
                //example: 2011-06-24,6.03,6.04,5.88,5.88,20465200,5.88
                if (fields.size() == StockModel::NUM_OF_ATTRIBUTES)
                {
                    _prices.prepend(decodeEntryStock(fields));
                }
            }
            qDebug() << "get stock data successfully, total:" << _prices.count() << "records.";
            emit downloadDone();*/

            this->updatePrices(reply);
         } else
         {
            qDebug() << "get stock data failed:" << reply->errorString();
            emit downloadFail();
         }
         reply->deleteLater();
         endResetModel();
         emit dataChanged(QModelIndex(), QModelIndex());
    }
}

QVariant StockModel::data(int index, int role) const {
    if (index < 0 || index > _prices.count())
        return QVariant();

    const StockPrice* price = _prices[index];
    if (role == StockModel::DateRole)
        return price->date();
    else if (role == StockModel::OpenPriceRole)
        return price->openPrice();
    else if (role == StockModel::ClosePriceRole)
        return price->closePrice();
    else if (role == StockModel::HighPriceRole)
        return price->highPrice();
    else if (role == StockModel::LowPriceRole)
        return price->lowPrice();
    else if (role == StockModel::AdjustedPriceRole)
        return price->adjustedPrice();
    else if (role == StockModel::VolumeRole)
        return price->volume();
    else if (role == StockModel::SectionRole)
        return price->date().year();
    return QVariant();
}

QVariant StockModel::data(QDate date, int role) const
{
    if ((_startDate > date) || (_endDate < date))
        return QVariant();

    int i;
    for (i=0;i<_prices.size() && _prices[i]->date() < date;i++);

    if (i>=(_prices.size()-1))
    {
        i = _prices.size()-2;
    }

    if (i<0)
    {
        i = 0;
    }

    const StockPrice* price = _prices[i];
    if (role == StockModel::DateRole)
        return price->date();
    else if (role == StockModel::OpenPriceRole)
        return price->openPrice();
    else if (role == StockModel::ClosePriceRole)
        return price->closePrice();
    else if (role == StockModel::HighPriceRole)
        return price->highPrice();
    else if (role == StockModel::LowPriceRole)
        return price->lowPrice();
    else if (role == StockModel::AdjustedPriceRole)
        return price->adjustedPrice();
    else if (role == StockModel::VolumeRole)
        return price->volume();
    else if (role == StockModel::SectionRole)
        return price->date().year();
    return QVariant();

}

QVariant StockModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() > _prices.count())
        return QVariant();

    const StockPrice* price = _prices[index.row()];
    if (role == StockModel::DateRole)
        return price->date();
    else if (role == StockModel::OpenPriceRole)
        return price->openPrice();
    else if (role == StockModel::ClosePriceRole)
        return price->closePrice();
    else if (role == StockModel::HighPriceRole)
        return price->highPrice();
    else if (role == StockModel::LowPriceRole)
        return price->lowPrice();
    else if (role == StockModel::AdjustedPriceRole)
        return price->adjustedPrice();
    else if (role == StockModel::VolumeRole)
        return price->volume();
    else if (role == StockModel::SectionRole)
        return price->date().year();    
    return QVariant();

}

QString StockModel::stockName() const
{
    return _stockName;
}
void StockModel::setStockName(const QString& name)
{
    if (_stockName != name) {
        _stockName = name;
        emit stockNameChanged();
    }
}

QDate StockModel::startDate() const
{
    return _startDate;
}
void StockModel::setStartDate(const QDate& date)
{
    if (_startDate.isValid() && _startDate != date) {
        _startDate = date;
        emit startDateChanged();
    }
}

QDate StockModel::endDate() const
{
    return _endDate;
}
void StockModel::setEndDate(const QDate& date)
{
    if (_endDate.isValid() && _endDate != date) {
        _endDate = date;
        emit endDateChanged();
    }
}

StockModel::StockDataCycle StockModel::dataCycle() const
{
    return _dataCycle;
}

QString StockModel::dataCycleString() const
{
    switch (_dataCycle) {
    case StockModel::Daily:
        return QString('d');
        break;
    case StockModel::Weekly:
        return QString('w');
    case StockModel::Monthly:
        return QString('m');
    case StockModel::Dividend:
        return QString('v');
    }

    return QString('d');
}

void StockModel::SetupFormatFile(QString header)
{
    attributesPosition.resize(StockModel::NUM_OF_ATTRIBUTES);
    attributesPosition.clear();

    header = header.toLower();
    QStringList fields = header.split(',');

    attributesPosition.append(fields.indexOf(QRegExp("^data.*")));
    attributesPosition.append(fields.indexOf(QRegExp("^open.*")));
    attributesPosition.append(fields.indexOf(QRegExp("^high.*")));
    attributesPosition.append(fields.indexOf(QRegExp("^low.*")));
    attributesPosition.append(fields.indexOf(QRegExp("^close.*")));
    attributesPosition.append(fields.indexOf(QRegExp("^volume.*")));
    attributesPosition.append(fields.indexOf(QRegExp("^adjusted.*")));

    //SetupFormatFile();
}

void StockModel::SetupFormatFile()
{
    attributesPosition.resize(StockModel::NUM_OF_ATTRIBUTES);
    attributesPosition.clear();
    for (int i = 0; i < StockModel::NUM_OF_ATTRIBUTES ; i++)
    {
        attributesPosition.append(i);
    }
}

void StockModel::updatePrices(QIODevice *inData)
{
    beginResetModel();

    foreach (StockPrice* p, _prices)
    {
        p->deleteLater();
    }
    _prices.clear();

    while (!inData->atEnd())
    {
        QString line = inData->readLine();
        QStringList fields = line.split(',');

        //data format:Date,Open,High,Low,Close,Volume,Adjusted close price
        //example: 2011-06-24,6.03,6.04,5.88,5.88,20465200,5.88
        if (fields.size() == StockModel::NUM_OF_ATTRIBUTES)
        {
            _prices.prepend(decodeEntryStock(fields));
        }
    }
    qDebug() << "get stock data successfully, total:" << _prices.count() << "records.";
    emit downloadDone();
}

StockPrice *StockModel::decodeEntryStock(QStringList &line)
{
    StockPrice* price = new StockPrice(this);

    if (-1 < attributesPosition[0])
    {
        price->setDate(QDate::fromString(line[attributesPosition[0]], Qt::ISODate));
    }

    if (-1 < attributesPosition[1])
    {
        price->setOpenPrice(line[attributesPosition[1]].toFloat());
    }

    if (-1 < attributesPosition[2])
    {
        price->setHighPrice(line[attributesPosition[2]].toFloat());
    }

    if (-1 < attributesPosition[3])
    {
        price->setLowPrice(line[attributesPosition[3]].toFloat());
    }

    if (-1 < attributesPosition[4])
    {
        price->setClosePrice(line[attributesPosition[4]].toFloat());
    }

    if (-1 < attributesPosition[5])
    {
        price->setVolume(line[attributesPosition[5]].toInt());
    }

    if (-1 < attributesPosition[6])
    {
        price->setAdjustedPrice(line[attributesPosition[6]].toFloat());
    }
    else
    {
        price->setAdjustedPrice(line[attributesPosition[4]].toFloat());
    }

    return price;
}

void StockModel::setDataCycle(StockModel::StockDataCycle cycle)
{
    if (_dataCycle != cycle) {
        _dataCycle = cycle;
        emit dataCycleChanged();
    }
}

StockModel::StockDataBase StockModel::dataBaseType() const
{
    return _dataBaseType;
}

void StockModel::setDataBaseType(StockModel::StockDataBase dataBase)
{
    if (_dataBaseType != dataBase)
    {
        _dataBaseType = dataBase;
        emit dataBaseTypeChanged();
    }
}

void StockModel::setFileName(QString file)
{
    this->fileName = file;
    this->setStockName(file.split("/").last());
}
