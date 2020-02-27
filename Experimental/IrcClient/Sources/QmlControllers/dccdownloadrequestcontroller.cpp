#include "Headers/QmlControllers/dccdownloadrequestcontroller.h"
#include <QDir>

DccDownloadRequestController::DccDownloadRequestController(QObject *parent) : QObject(parent)
{

}

void DccDownloadRequestController::registerDccClient(DccClient *dccClient)
{
    connect(dccClient, SIGNAL(downloadRequest(DccDownloadObj*)), this, SLOT(onDownloadRequestSlot(DccDownloadObj*)));
    connect(this, SIGNAL(acceptDownloadRequestSignal(DccDownloadObj*)), dccClient, SLOT(acceptDownload(DccDownloadObj*)));
}

void DccDownloadRequestController::onDownloadRequestSlot(DccDownloadObj *downloadObj)
{
    qDebug() << "Got download request for file: " << downloadObj->getFileName();
    emit this->downloadRequestSignal(downloadObj);
}

void DccDownloadRequestController::acceptDownloadRequestSlot(DccDownloadObj *downloadObj)
{
    qDebug() << "Accepted download request for file: " << downloadObj->getFileName();
    emit this->acceptDownloadRequestSignal(downloadObj);
}

void DccDownloadRequestController::changeDownloadDirectorySlot(DccDownloadObj *downloadObj, QString downloadDirectory)
{
    qDebug() << "Changing download directory for file: " << downloadObj->getFileName() << " From " << downloadObj->getDownloadDirPath();
    downloadObj->setDownloadDirPath(downloadDirectory);
    qDebug() << "Changing download directory for file: " << downloadObj->getFileName() << " To " << downloadObj->getDownloadDirPath();
}
