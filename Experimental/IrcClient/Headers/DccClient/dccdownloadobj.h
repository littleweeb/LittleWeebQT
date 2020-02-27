#ifndef DCCDOWNLOADOBJ_H
#define DCCDOWNLOADOBJ_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QThread>
#include <QQuickItem>
#include "dccdownloadprogress.h"
#include "dccdownloadstatus.h"


class DccDownloadObj : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString downloadId READ getDownloadId WRITE setDownloadId NOTIFY downloadIdChanged)
    Q_PROPERTY(quint32 serverIp READ getServerIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(quint32 serverPort READ getServerPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(quint64 fileSize READ getFileSize WRITE setFileSize NOTIFY fileSizeChanged)
    Q_PROPERTY(QString downloadDirPath READ getDownloadDirPath WRITE setDownloadDirPath NOTIFY downloadDirPathChanged)
    Q_PROPERTY(QDir *downloadDir READ getDownloadDir WRITE setDownloadDir)
    Q_PROPERTY(DccDownloadProgress *progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(DccDownloadStatus *status READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(DccDownloadObj *currentInstance READ getCurrentInstance WRITE setCurrentInstance NOTIFY currentInstanceChanged)
    Q_PROPERTY(QFile *fileToWrite READ getFileToWrite WRITE setFileToWrite)
    Q_PROPERTY(QThread *downloaderThread READ getDownloaderThread WRITE setDownloaderThread)

public:
    explicit DccDownloadObj(QObject *parent = nullptr);
    QString getDownloadId() const;
    quint32 getServerIp() const;
    QString getFileName() const;
    quint32 getServerPort() const;
    quint64 getFileSize() const;
    QString getDownloadDirPath() const;
    DccDownloadProgress* getProgress() const;
    DccDownloadStatus* getStatus() const;
    DccDownloadObj* getCurrentInstance() const; // THIS FEELS HACKY AS FUCK
    QFile* getFileToWrite() const;
    QDir* getDownloadDir() const;
    QThread* getDownloaderThread() const;
    ~DccDownloadObj();


public slots:
    void setDownloadId(QString downloadId);
    void setServerIp(quint32 serverIp);
    void setFileName(QString fileName);
    void setServerPort(quint32 serverPort);
    void setFileSize(quint64 fileSize);
    void setDownloadDirPath(QString downloadDirPath);
    void setProgress(DccDownloadProgress* progress);
    void setStatus(DccDownloadStatus* status);
    void setCurrentInstance(DccDownloadObj* currentInstance); // THIS FEELS HACKY AS FUCK
    void setFileToWrite(QFile* fileToWrite);
    void setDownloadDir(QDir* downloadDir);
    void setDownloaderThread(QThread* downloaderThread);

signals:
    void downloadIdChanged(QString downloadId);
    void serverIpChanged(quint32 serverIp);
    void fileNameChanged(QString fileName);
    void serverPortChanged(quint32 serverPort);
    void fileSizeChanged(quint64 fileSize);
    void downloadDirPathChanged(QString downloadDirPath);
    void progressChanged(DccDownloadProgress* progress);
    void statusChanged(DccDownloadStatus* status);
    void currentInstanceChanged(DccDownloadObj* currentInstance);

private:
    DccDownloadObj* m_currentInstance = nullptr; // THIS FEELS HACKY AS FUCK
    QString m_downloadId;
    quint32 m_serverIp;
    QString m_fileName;
    quint32 m_serverPort;
    quint64 m_fileSize;
    QString m_downloadDirPath;
    DccDownloadProgress* m_progress;
    DccDownloadStatus* m_status;
    QFile *m_fileToWrite;
    QDir *m_downloadDir;
    QThread *m_downloaderThread;
};

#endif // DCCDOWNLOADOBJ_H
