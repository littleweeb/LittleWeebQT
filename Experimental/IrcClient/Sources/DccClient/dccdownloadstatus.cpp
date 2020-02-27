#include "Headers/DccClient/dccdownloadstatus.h"

QString DccDownloadStatus::getStatus() const
{
    return m_status;
}

DccDownloadState DccDownloadStatus::getState() const
{
    return m_state;
}

void DccDownloadStatus::setStatus(QString status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(status);
}

void DccDownloadStatus::setState(DccDownloadState state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(m_state);
}
