#ifndef DCCDOWNLOADPROGRESS_H
#define DCCDOWNLOADPROGRESS_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QThread>
#include <QQuickItem>

class DccDownloadProgress : public QObject {
    Q_OBJECT
    Q_PROPERTY(quint64 progress READ getProgress WRITE setProgress NOTIFY progressChanged)
    Q_PROPERTY(quint64 speed READ getSpeed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(quint64 bytesDownloaded READ getBytesDownloaded WRITE setBytesDownloaded NOTIFY bytesDownloadedChanged)
    quint64 m_progress;
    quint64 m_speed;
    quint64 m_bytesDownloaded;

public:
    quint64 getProgress() const;
    quint64 getSpeed() const;
    quint64 getBytesDownloaded() const;

public slots:
    void setProgress(quint64 progress);
    void setSpeed(quint64 speed);
    void setBytesDownloaded(quint64 bytesDownloaded);
signals:
    void progressChanged(quint64 progress);
    void speedChanged(quint64 speed);
    void bytesDownloadedChanged(quint64 bytesDownloaded);
};

#endif // DCCDOWNLOADPROGRESS_H
