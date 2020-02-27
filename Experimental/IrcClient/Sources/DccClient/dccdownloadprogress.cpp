#include "Headers/DccClient/dccdownloadprogress.h"
quint64 DccDownloadProgress::getProgress() const
{
    return m_progress;
}

quint64 DccDownloadProgress::getSpeed() const
{
    return m_speed;
}

quint64 DccDownloadProgress::getBytesDownloaded() const
{
    return m_bytesDownloaded;
}

void DccDownloadProgress::setProgress(quint64 progress)
{
    if (m_progress == progress)
        return;

    m_progress = progress;
    emit progressChanged(m_progress);
}

void DccDownloadProgress::setSpeed(quint64 speed)
{
    if (m_speed == speed)
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}

void DccDownloadProgress::setBytesDownloaded(quint64 bytesDownloaded)
{
    if (m_bytesDownloaded == bytesDownloaded)
        return;

    m_bytesDownloaded = bytesDownloaded;
    emit bytesDownloadedChanged(m_bytesDownloaded);
}
