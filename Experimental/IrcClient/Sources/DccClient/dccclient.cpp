#include <QRegularExpression>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QFile>
#include <QStandardPaths>
#include <QHostAddress>
#include "Headers/DccClient/dccclient.h"
#include "Headers/DccClient/dccdownloader.h"

DccClient::DccClient(QObject *parent) : QObject(parent)
{
}

void DccClient::parseDccRequest(QString aInput)
{
    qDebug() << "Parsing dcc request for: " << aInput;
    QRegularExpression re("(?P<user>[^\\s]+)!(?P<userhost>[^\\s]+)\\sPRIVMSG\\s(?P<channel>[^\\s]+)\\s:.*(?<=DCC\\sSEND)\\s(?P<filename>.*)\\s(?P<ip>[0-9]+)\\s(?P<port>[0-9]+)\\s(?P<filesize>[0-9]+)");
    QRegularExpressionMatch match = re.match(aInput);

    if(match.hasMatch())
    {
        qDebug() << "This input: " << aInput << "Matched the DCC download accept request";
        qDebug() << "Server: " << match.captured("ip") << ":" << match.captured("port");
        DccDownloadObj *newDownload = new DccDownloadObj;
        newDownload->setFileName(match.captured("filename"));

        // In case file contains quotes remove them.
        if(newDownload->getFileName().startsWith("\"") && newDownload->getFileName().endsWith("\""))
        {
            newDownload->setFileName(newDownload->getFileName().remove(0, 1));
            newDownload->setFileName(newDownload->getFileName().remove(newDownload->getFileName().length() - 1, 1));
        }

        bool success = false;
        newDownload->setFileSize(match.captured("filesize").toULongLong(&success));

        if (!success)
        {
            newDownload->getStatus()->setState(DccDownloadState::Failed);
            newDownload->getStatus()->setStatus(QString("Failed converting pFileSize: %0").arg(match.captured("filesize")));
            emit downloadUpdate(newDownload);
            qDebug() << newDownload->getStatus()->getStatus();
            return;
        }

        newDownload->setServerIp(match.captured("ip").toULong(&success));
        if (!success)
        {
            newDownload->getStatus()->setState(DccDownloadState::Failed);
            newDownload->getStatus()->setStatus(QString("Failed converting pServerIp: %0").arg(match.captured("ip")));
            emit downloadUpdate(newDownload);
            qDebug() << newDownload->getStatus()->getStatus();
            return;
        }
        newDownload->setServerPort(match.captured("port").toULong(&success));
        if (!success)
        {
            newDownload->getStatus()->setState(DccDownloadState::Failed);
            newDownload->getStatus()->setStatus(QString("Failed converting pServerPort: %0").arg(match.captured("port")));
            emit downloadUpdate(newDownload);
            qDebug() << newDownload->getStatus()->getStatus();
            return;
        }

        qDebug() << "Server: " << newDownload->getServerIp() << ":" << newDownload->getServerPort();
        newDownload->getStatus()->setState(DccDownloadState::ReceivedRequest);
        emit downloadUpdate(newDownload);

        newDownload->setDownloadDirPath(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));

        qDebug() << "Appended download for file: " << newDownload->getFileName() << " of size " << newDownload->getFileSize() << "to queue";
        this->downloadQueue.append(newDownload);
        emit downloadRequest(newDownload);
    }
}

void DccClient::acceptDownload(DccDownloadObj* aDownloadObject)
{
    qDebug() << "Got accepted download: " << aDownloadObject->getFileName();
    DccDownloadObj *toDownload = nullptr;
    for(int i = 0; i < this->downloadQueue.length(); i++)
    {
        if(this->downloadQueue[i] == aDownloadObject)
        {
            toDownload = aDownloadObject;
            break;
        }
        else
        {
            qDebug() << "Cannot accept download with id: " << aDownloadObject->getDownloadId() << " for file: " << aDownloadObject->getFileName() << " as it is not equal to one of the queued downloads: " << this->downloadQueue[i]->getDownloadId() << " for file: " << this->downloadQueue[i]->getFileName();
        }
    }

    if(toDownload == nullptr)
    {
        qDebug() << "No download matched download in queue!";
    }

    qDebug() << "Got VALID accepted download: " << aDownloadObject->getFileName();

    DccDownloader *downloader = new DccDownloader;
    downloader->registerParentClient(this);
    downloader->setDownload(aDownloadObject);
    connect(downloader, &QThread::finished, downloader, &QObject::deleteLater);
    qDebug() << "Starting the downloader (from dccclient) " << aDownloadObject->getFileName();
    downloader->start();

    // Bind thread to the download object (to ensure it lives as long as the download entry exist (tis but a brainfart)
    aDownloadObject->setDownloaderThread(downloader);
}

void DccClient::onDownloadUpdate(DccDownloadObj *aDownloadObject)
{
    qDebug() << "Got download update :D " << aDownloadObject->getFileName();
    emit this->downloadUpdate(aDownloadObject);
}

void DccClient::stopDownload(DccDownloadObj *aDownloadObject)
{
    emit this->stopDownloadSignal(aDownloadObject);
}

void DccClient::onDownloadFinished(DccDownloadObj *aDownloadObject)
{
    qDebug() << "Dccclient received finished download signal";
    for(int i = 0; i < this->downloadQueue.length(); i++)
    {
        if(this->downloadQueue[i] == aDownloadObject)
        {
            this->downloadQueue.removeAt(i);
            this->downloadsFinished.append(aDownloadObject);

            if(aDownloadObject->getDownloaderThread()->isRunning())
            {
                aDownloadObject->getDownloaderThread()->quit();
            }
            break;
        }
    }
}

DccClient::~DccClient()
{
    for(int i = 0; i < this->downloadQueue.length(); i++)
    {
        if(this->downloadQueue[i] != nullptr)
        {
            delete this->downloadQueue[i];
        }
    }

    for(int i = 0; i < this->downloadsFinished.length(); i++)
    {
        if(this->downloadsFinished[i] != nullptr)
        {
            delete this->downloadsFinished[i];
        }
    }
}
