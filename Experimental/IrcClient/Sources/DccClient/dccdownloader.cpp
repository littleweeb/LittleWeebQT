#include "Headers/DccClient/dccdownloader.h"
#include <QRegularExpression>
#include <QDebug>
#include <QTimer>
#include <QEventLoop>
#include <QFile>
#include <QStandardPaths>
#include <QHostAddress>

void DccDownloader::run()
{

    qDebug() << "Starting this download: " << this->currentDownload->getFileName();
    this->currentDownload->getStatus()->setState(DccDownloadState::ReceivedRequest);

    this->currentDownload->setDownloadDir(new QDir(this->currentDownload->getDownloadDirPath()));

    if(!this->currentDownload->getDownloadDir()->exists())
        this->currentDownload->getDownloadDir()->mkdir(this->currentDownload->getDownloadDirPath());

    this->currentDownload->setFileToWrite(new QFile(this->currentDownload->getDownloadDir()->absoluteFilePath(this->currentDownload->getFileName())));

    qDebug() << "Succesfully created dirs/newfile: " << this->currentDownload->getFileName();
    // Open file for read/write action, in Append mode so bytes get appended by onReadyRead
    if(!this->currentDownload->getFileToWrite()->open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        this->currentDownload->getStatus()->setStatus(QString("Could not start download: %0 could not be opened!").arg(this->currentDownload->getDownloadDir()->absoluteFilePath(this->currentDownload->getFileName())));
        qDebug() << "Failed to open file: " << this->currentDownload->getStatus()->getStatus();
        this->currentDownload->getStatus()->setState(DccDownloadState::Failed);
        return;
    }

    qDebug() << "Succesfully opened file file: " << this->currentDownload->getFileName() << " Preparing to writh the last bit of a total file size: " << this->currentDownload->getFileSize();

    this->currentDownload->getStatus()->setState(DccDownloadState::RequestVerified);
    qDebug() << "Ready to send download update :D";
    emit downloadUpdate(currentDownload);

    quint64 previousPosition = 0;
    quint64 currentPosition = 0;
    quint64 progress = 0;
    quint64 bytesWritten = 0;
    quint8 timeOutCounter = 0;

    do{
        bool finished = false;
        switch(this->currentDownload->getStatus()->getState())
        {
            case DccDownloadState::Waiting:
                previousPosition = 0;
                currentPosition = 0;
                progress = 0;
                bytesWritten = 0;
                timeOutCounter++;
                break;
            case DccDownloadState::ReceivedRequest:
                break;
            case DccDownloadState::RequestVerified:
                emit this->connectToServerSignal();
                this->currentDownload->getStatus()->setState(DccDownloadState::Waiting);
                break;
            case DccDownloadState::Downloading:
                timeOutCounter = 0;
                currentPosition = this->currentDownload->getFileToWrite()->pos();
                this->currentDownload->getProgress()->setBytesDownloaded(currentPosition);
                this->currentDownload->getProgress()->setSpeed(((currentPosition - previousPosition) * 10));
                this->currentDownload->getProgress()->setProgress(static_cast<quint64>((static_cast<double>(currentPosition) / static_cast<double>(this->currentDownload->getFileSize())) * 100));

                qDebug() << "Download update, speed: " << this->currentDownload->getProgress()->getSpeed() << ", downloaded" << this->currentDownload->getProgress()->getBytesDownloaded() << " off " << this->currentDownload->getFileSize() << ", totall progress: " << this->currentDownload->getProgress()->getProgress();
                previousPosition = currentPosition;

                if(this->currentDownload->getFileToWrite()->pos() >= static_cast<qint64>(this->currentDownload->getFileSize()))
                {
                    this->currentDownload->getStatus()->setState(DccDownloadState::FinishedDownload);
                }
                break;
            case DccDownloadState::Failed:
            case DccDownloadState::Aborted:
            case DccDownloadState::FinishedDownload:
                qDebug() << "FAILED/FINISHED DOWNLOAD";
                this->currentDownload->getFileToWrite()->close();
                finished = true;
                break;
        }
        emit this->downloadUpdate(this->currentDownload);
        QThread::msleep(100);
        if(finished)
            break;
    } while(timeOutCounter < (this->timeout / 100));

    if(timeOutCounter >= (this->timeout / 100))
    {
        this->currentDownload->getStatus()->setState(DccDownloadState::Failed);
        this->currentDownload->getStatus()->setStatus("Timeout waiting for task switch from DORMANT state.");
        emit this->downloadUpdate(this->currentDownload);
    }

    emit this->downloadFinished(this->currentDownload);
}

DccDownloader::DccDownloader(QObject *parent)
{
    (void) parent;
    connect(this, SIGNAL(connectToServerSignal()), this, SLOT(connectToServerSlot()));
    connect(this, SIGNAL(writeFileSignal(QByteArray)), this, SLOT(writeFile(QByteArray)));
}

void DccDownloader::registerParentClient(DccClient* parent)
{
    connect(this, SIGNAL(downloadUpdate(DccDownloadObj*)), parent, SLOT(onDownloadUpdate(DccDownloadObj*)));
    connect(this, SIGNAL(downloadFinished(DccDownloadObj*)), parent, SLOT(onDownloadFinished(DccDownloadObj*)));
    connect(parent, SIGNAL(stopDownloadSignal(DccDownloadObj*)), this, SLOT(onStopDownload(DccDownloadObj*)));
}

void DccDownloader::setDownload(DccDownloadObj *aDownloadObject)
{
    this->currentDownload = aDownloadObject;
}

void DccDownloader::connectToServerSlot()
{
    qDebug() << "Starting dcc connection to server: " << this->currentDownload->getServerIp() << ":" << this->currentDownload->getServerPort();
    this->pTcpSocket = new QTcpSocket(this);
    connect(this->pTcpSocket, SIGNAL(connected()),this, SLOT(onTcpConnect()));
    connect(this->pTcpSocket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnect()));
    connect(this->pTcpSocket, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
    connect(this->pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onTcpError(QAbstractSocket::SocketError)));
    qDebug() << "Connected to all necessary slots: ";
    QHostAddress address = QHostAddress(this->currentDownload->getServerIp());
    qDebug() << "Connection to server: " << address.toString() << ":" << this->currentDownload->getServerPort();

    this->pTcpSocket->connectToHost(address, this->currentDownload->getServerPort());

    // Half of total timeout, setting up the download although unlikely will take a bit of the timeout as well.
    if(!this->pTcpSocket->waitForConnected(this->timeout / 2))
    {
        this->currentDownload->getStatus()->setState(DccDownloadState::Failed);
        this->currentDownload->getStatus()->setStatus(QString("Error: %0").arg(this->pTcpSocket->errorString()));

        qDebug() << this->currentDownload->getStatus()->getStatus();
        return;
    }
    qDebug() << "Succesfully connected to server: " << this->currentDownload->getServerIp() << ":" << this->currentDownload->getServerPort();

}

void DccDownloader::onStopDownload(DccDownloadObj *aDownloadObject)
{
    if(aDownloadObject == this->currentDownload)
    {
        if(this->currentDownload->getStatus()->getState() == DccDownloadState::Downloading)
        {
            //this->pTcpSocket->disconnectFromHost();
            this->currentDownload->getFileToWrite()->remove();
        }
        this->currentDownload->getStatus()->setState(DccDownloadState::Aborted);
        this->currentDownload->getStatus()->setStatus("Download aborted by user.");
    }
}

void DccDownloader::onTcpConnect()
{
    this->currentDownload->getStatus()->setState(DccDownloadState::Downloading);
    this->currentDownload->getStatus()->setStatus(QString("Connected to DCC Server!"));
    qDebug() << this->currentDownload->getStatus()->getStatus();
}

void DccDownloader::onTcpDisconnect()
{
    if(this->currentDownload->getStatus()->getState() == DccDownloadState::Downloading)
    {
        this->currentDownload->getFileToWrite()->remove();
        this->currentDownload->getStatus()->setState(DccDownloadState::Failed);
        this->currentDownload->getStatus()->setStatus("Got unexpectedly disconnected from dcc server, while downloading.");
    }
    if(this->currentDownload->getStatus()->getState() != DccDownloadState::Aborted)
    {
        this->currentDownload->getStatus()->setState(DccDownloadState::Failed);
        this->currentDownload->getStatus()->setStatus("Got unexpectedly disconnected from dcc server, while downloading.");
    }
    qDebug() << this->currentDownload->getStatus()->getStatus();
}

void DccDownloader::onTcpError(QAbstractSocket::SocketError error)
{
    qDebug() << "GOT TCP ERROR :( : " << error;
}

void DccDownloader::onReadyRead()
{
    if(this->pTcpSocket->isReadable() && this->pTcpSocket->bytesAvailable() > 0)
    {
        emit this->writeFileSignal(this->pTcpSocket->readAll());
    }
}

void DccDownloader::writeFile(QByteArray data)
{
    if(!this->currentDownload->getFileToWrite()->isWritable())
    {
        this->currentDownload->getStatus()->setState(DccDownloadState::FinishedDownload);
        //this->pTcpSocket->disconnectFromHost();
        qDebug() << "Could not write bytes to QFile";
        return;
    }
    this->currentDownload->getFileToWrite()->write(data);
    this->currentDownload->getFileToWrite()->flush();
}
