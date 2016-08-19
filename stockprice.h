#ifndef STOCKPRICE_H
#define STOCKPRICE_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QDate>

class StockPrice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDate date READ date)
    Q_PROPERTY(qreal openPrice READ openPrice)
    Q_PROPERTY(qreal closePrice READ closePrice)
    Q_PROPERTY(qreal highPrice READ highPrice)
    Q_PROPERTY(qreal lowPrice READ lowPrice)
    Q_PROPERTY(qint32 volume READ volume)
    Q_PROPERTY(qreal adjustedPrice READ adjustedPrice)
public:

    StockPrice(QObject *parent = 0)
        : QObject(parent)
        , _openPrice(-1)
        , _closePrice(-1)
        , _highPrice(-1)
        , _lowPrice(-1)
        , _volume(-1)
        , _adjustedPrice(-1)
    {
    }
    QDate date() const {return _date;}
    qreal openPrice() const {return _openPrice; }
    qreal closePrice() const {return _closePrice;}
    qreal highPrice() const {return _highPrice;}
    qreal lowPrice() const{return _lowPrice;}
    qreal adjustedPrice() const{return _adjustedPrice;}
    qint32 volume() const{return _volume;}

    void setDate(const QDate& date){_date = date;}
    void setOpenPrice(qreal price){_openPrice = price;}
    void setClosePrice(qreal price){_closePrice = price;}
    void setHighPrice(qreal price){_highPrice = price;}
    void setLowPrice(qreal price){_lowPrice = price;}
    void setAdjustedPrice(qreal price) {_adjustedPrice = price;}
    void setVolume(qint32 volume) {_volume = volume;}

private:
    QDate _date;
    qreal _openPrice;
    qreal _closePrice;
    qreal _highPrice;
    qreal _lowPrice;
    qint32 _volume;
    qreal _adjustedPrice;
};

class QNetworkReply;
class QNetworkAccessManager;
class StockModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString stockName READ stockName WRITE setStockName NOTIFY stockNameChanged)
    Q_PROPERTY(QDate startDate READ startDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QDate endDate READ endDate WRITE setEndDate NOTIFY endDateChanged)
    Q_PROPERTY(StockDataCycle dataCycle READ dataCycle WRITE setDataCycle NOTIFY dataCycleChanged)

    Q_ENUMS(StockDataCycle)
public:
    enum StockDataCycle {
        Daily,
        Weekly,
        Monthly,
        Dividend
    };

    enum StockModelRoles {
        DateRole = Qt::UserRole + 1,
        SectionRole,
        OpenPriceRole,
        ClosePriceRole,
        HighPriceRole,
        LowPriceRole,
        VolumeRole,
        AdjustedPriceRole
    };

    StockModel(QObject *parent = 0);

    QString stockName() const;
    void setStockName(const QString& name);

    QDate startDate() const;
    void setStartDate(const QDate& date);

    QDate endDate() const;
    void setEndDate(const QDate& date);

    StockDataCycle dataCycle() const;
    void setDataCycle(StockDataCycle cycle);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant data(int index, int role) const;
    QVariant data(QDate date, int role) const;

signals:
    void downloadDone();
    void downloadFail();
    void stockNameChanged();
    void startDateChanged();
    void endDateChanged();
    void dataCycleChanged();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    void requestData();
    StockPrice* stockPriceAtIndex(int idx) const;
private slots:
    void doRequest();
    void update(QNetworkReply* reply);
private:
    QString dataCycleString() const;
    QList<StockPrice*> _prices;
    QString _stockName;
    QDate _startDate;
    QDate _endDate;
    StockDataCycle _dataCycle;
    QNetworkAccessManager* _manager;
    bool _updating;
};

#endif // STOCKPRICE_H
