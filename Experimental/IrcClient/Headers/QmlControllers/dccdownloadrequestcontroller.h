#ifndef DCCDOWNLOADREQUESTCONTROLLER_H
#define DCCDOWNLOADREQUESTCONTROLLER_H

#include <QObject>
#include "Headers/DccClient/dccclient.h"
class DccDownloadRequestController : public QObject
{
    Q_OBJECT
public:
    explicit DccDownloadRequestController(QObject *parent = nullptr);
    void registerDccClient(DccClient *dccClient);
public slots:
    void onDownloadRequestSlot(DccDownloadObj *downloadObj);
    void acceptDownloadRequestSlot(DccDownloadObj *downloadObj);
    void changeDownloadDirectorySlot(DccDownloadObj *downloadObj, QString downloadDirectory);
signals:
    void downloadRequestSignal(DccDownloadObj *downloadObj);
    void acceptDownloadRequestSignal(DccDownloadObj *downloadObj);
private:
    DccClient *dccClient;
};

#endif // DCCDOWNLOADREQUESTCONTROLLER_H
