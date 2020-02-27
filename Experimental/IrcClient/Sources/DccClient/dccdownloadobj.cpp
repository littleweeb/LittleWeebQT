#include "Headers/DccClient/dccdownloadobj.h"

DccDownloadObj::DccDownloadObj(QObject *parent)
{
   qDebug() << "I got created: " << this;
   this->m_progress = new DccDownloadProgress();
   this->m_status = new DccDownloadStatus();
}

QString DccDownloadObj::getDownloadId() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getDownloadId();
    return m_downloadId;
}

quint32 DccDownloadObj::getServerIp() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getServerIp();
    return m_serverIp;
}

QString DccDownloadObj::getFileName() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getFileName();
    return m_fileName;
}

quint32 DccDownloadObj::getServerPort() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getServerPort();
    return m_serverPort;
}

quint64 DccDownloadObj::getFileSize() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getFileSize();
    return m_fileSize;
}

QString DccDownloadObj::getDownloadDirPath() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getDownloadDirPath();
    return m_downloadDirPath;
}

DccDownloadProgress* DccDownloadObj::getProgress() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getProgress();
    return m_progress;
}

DccDownloadStatus* DccDownloadObj::getStatus() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getStatus();
    return m_status;
}

DccDownloadObj *DccDownloadObj::getCurrentInstance() const
{
    qDebug() << "Get current instance ";
    return m_currentInstance;
}

QFile *DccDownloadObj::getFileToWrite() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getFileToWrite();
    return m_fileToWrite;
}

QDir *DccDownloadObj::getDownloadDir() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getDownloadDir();
    return m_downloadDir;
}

QThread *DccDownloadObj::getDownloaderThread() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getDownloaderThread();
    return m_downloaderThread;
}

void DccDownloadObj::setDownloadId(QString downloadId)
{
    if(m_currentInstance != nullptr)
    {
        m_currentInstance->setDownloadId(downloadId);
        return;
    }

    if (m_downloadId == downloadId)
        return;

    m_downloadId = downloadId;
    emit downloadIdChanged(m_downloadId);
}

void DccDownloadObj::setServerIp(quint32 serverIp)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setServerIp(serverIp);

    if (m_serverIp == serverIp)
        return;

    m_serverIp = serverIp;
    emit serverIpChanged(m_serverIp);
}

void DccDownloadObj::setFileName(QString fileName)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setFileName(fileName);

    if (m_fileName == fileName)
        return;

    m_fileName = fileName;
    emit fileNameChanged(m_fileName);
}

void DccDownloadObj::setServerPort(quint32 serverPort)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setServerPort(serverPort);

    if (m_serverPort == serverPort)
        return;

    m_serverPort = serverPort;
    emit serverPortChanged(m_serverPort);
}

void DccDownloadObj::setFileSize(quint64 fileSize)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setFileSize(fileSize);

    if (m_fileSize == fileSize)
        return;

    m_fileSize = fileSize;
    emit fileSizeChanged(m_fileSize);
}

void DccDownloadObj::setDownloadDirPath(QString downloadDirPath)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setDownloadDirPath(downloadDirPath);

    if (m_downloadDirPath == downloadDirPath)
        return;

    m_downloadDirPath = downloadDirPath;
    emit downloadDirPathChanged(m_downloadDirPath);
}

void DccDownloadObj::setProgress(DccDownloadProgress* progress)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setProgress(progress);

    if (m_progress == progress)
        return;

    m_progress = progress;
    emit progressChanged(m_progress);
}

void DccDownloadObj::setStatus(DccDownloadStatus* status)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setStatus(status);

    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(m_status);
}

void DccDownloadObj::setCurrentInstance(DccDownloadObj *currentInstance)
{
    if (m_currentInstance == currentInstance)
        return;

    qDebug() << "I NOW posses another instance (null if not): " << m_currentInstance;
    m_currentInstance = currentInstance;
}

void DccDownloadObj::setFileToWrite(QFile *fileToWrite)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setFileToWrite(fileToWrite);

    if (m_fileToWrite == fileToWrite)
        return;

    m_fileToWrite = fileToWrite;
}

void DccDownloadObj::setDownloadDir(QDir *downloadDir)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setDownloadDir(downloadDir);

    if (m_downloadDir == downloadDir)
        return;

    m_downloadDir = downloadDir;
}

void DccDownloadObj::setDownloaderThread(QThread *downloaderThread)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setDownloaderThread(downloaderThread);

    if (m_downloaderThread == downloaderThread)
        return;

    m_downloaderThread = downloaderThread;
}



DccDownloadObj::~DccDownloadObj()
{
    qDebug() << "I posses another instance (null if not): " << m_currentInstance;
    qDebug() << "I got destructed: " << this;
}
