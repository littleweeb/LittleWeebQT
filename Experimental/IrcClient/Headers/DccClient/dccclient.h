#ifndef DCCCLIENT_H
#define DCCCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QThread>
#include "dccdownloadobj.h"

class DccClient : public QObject
{
    Q_OBJECT
public:
    explicit DccClient(QObject *parent = nullptr);
    void setDownloadDirectory(QString aDownloadDirectory);
    ~DccClient();
private:
    QList<DccDownloadObj*> downloadQueue;
    QList<DccDownloadObj*> downloadsFinished;
public slots:
    void parseDccRequest(QString aInput);
    void acceptDownload(DccDownloadObj *aDownloadObject);
    void onDownloadUpdate(DccDownloadObj* aDownloadObject);
    void stopDownload(DccDownloadObj* aDownloadObject);
    void onDownloadFinished(DccDownloadObj* aDownloadObject);
signals:
    void downloadRequest(DccDownloadObj* aDownloadObject);
    void stopDownloadSignal(DccDownloadObj* aDownloadObject);
    void downloadUpdate(DccDownloadObj* aDownloadObject);
};

#endif // DCCCLIENT_H
