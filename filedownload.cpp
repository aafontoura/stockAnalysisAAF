#include "filedownload.h"
#include <QCoreApplication>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QFile>
#include <QDebug>

FileDownload::FileDownload() {
    connect(&manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(downloadFinished(QNetworkReply*)));
}


void FileDownload::setTarget(const QString &t) {
    this->target = t;
}

void FileDownload::downloadFinished(QNetworkReply *data) {

    QFile localFile("downloadedfile.csv");
    if (!localFile.open(QIODevice::WriteOnly))
        return;
    const QByteArray sdata = data->readAll();
    localFile.write(sdata);
    qDebug() << this->target;
    qDebug() << sdata;
    localFile.close();

    emit done();
}

void FileDownload::download() {
    QUrl url = QUrl::fromEncoded(this->target.toLocal8Bit());
    QNetworkRequest request(url);
    qDebug() << url;
    QObject::connect(manager.get(request), SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));

}

void FileDownload::downloadProgress(qint64 recieved, qint64 total) {
    qDebug() << recieved << total;
}

