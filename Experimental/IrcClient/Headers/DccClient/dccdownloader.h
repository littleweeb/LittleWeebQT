#ifndef DCCDOWNLOADER_H
#define DCCDOWNLOADER_H

#include <QObject>
#include <QTcpSocket>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QThread>

#include "dccclient.h"

class DccDownloader : public QThread
{
    Q_OBJECT
    void run();
public:
    explicit DccDownloader(QObject *parent = nullptr);
    void registerParentClient(DccClient* parent);
    void setDownload(DccDownloadObj*);
private:
    QTcpSocket *pTcpSocket;
    const static int retry = 3;
    const static qint64 timeout = 9000;
    DccDownloadObj* currentDownload;
public slots:
    void connectToServerSlot();
    void onStopDownload(DccDownloadObj* aDownloadObject);
    void onTcpConnect();
    void onTcpDisconnect();
    void onTcpError(QAbstractSocket::SocketError);
    void onReadyRead();
private slots:
    void writeFile(QByteArray data);
signals:
    void connectToServerSignal();
    void writeFileSignal(QByteArray data);
    void downloadUpdate(DccDownloadObj* aDownloadObject);
    void downloadFinished(DccDownloadObj* aDownloadObject);
};


#endif // DCCDOWNLOADER_H
