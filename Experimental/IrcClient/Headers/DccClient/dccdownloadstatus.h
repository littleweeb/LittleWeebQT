#ifndef DCCDOWNLOADSTATUS_H
#define DCCDOWNLOADSTATUS_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QThread>
#include <QQuickItem>


enum class DccDownloadState{
    Waiting,
    ReceivedRequest,
    RequestVerified,
    Downloading,
    FinishedDownload,
    Failed,
    Aborted
};

class DccDownloadStatus : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString status READ getStatus WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(enum DccDownloadState state READ getState WRITE setState NOTIFY stateChanged)

    QString m_status;
    DccDownloadState m_state;
public:
    QString getStatus() const;
    DccDownloadState getState() const;
public slots:
    void setStatus(QString status);
    void setState(DccDownloadState state);
signals:
    void statusChanged(QString status);
    void stateChanged(DccDownloadState state);
};
#endif // DCCDOWNLOADSTATUS_H

