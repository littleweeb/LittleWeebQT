#include "Headers/IrcClient/ircserver.h"

QString IrcServer::getServerIp() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getServerIp();
    return m_serverIp;
}

QString IrcServer::getServerPort() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getServerPort();
    return m_serverPort;
}

bool IrcServer::getUseSsl() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getUseSsl();
    return m_useSsl;
}

QList<QString> IrcServer::getChannels() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getChannels();
    return m_channels;
}

QString IrcServer::getUserName() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getUserName();
    return m_userName;
}

QString IrcServer::getNickName() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getNickName();
    return m_nickName;
}

IrcServer* IrcServer::getCurrentInstance() const
{
    return m_currentInstance;
}

void IrcServer::setServerIp(QString serverIp)
{

    if(m_currentInstance != nullptr)
        return m_currentInstance->setServerIp(serverIp);

    if (m_serverIp == serverIp)
        return;

    m_serverIp = serverIp;
    emit serverIpChanged(m_serverIp);
}

void IrcServer::setServerPort(QString serverPort)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setServerPort(serverPort);

    if (m_serverPort == serverPort)
        return;

    m_serverPort = serverPort;
    emit serverPortChanged(m_serverPort);
}

void IrcServer::setUseSsl(bool useSsl)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setUseSsl(useSsl);

    if (m_useSsl == useSsl)
        return;

    m_useSsl = useSsl;
    emit useSslChanged(m_useSsl);
}

void IrcServer::setChannels(QList<QString> channels)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setChannels(channels);

    if (m_channels == channels)
        return;

    m_channels = channels;
    emit channelsChanged(m_channels);
}

void IrcServer::setUserName(QString userName)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setUserName(userName);

    if (m_userName == userName)
        return;

    m_userName = userName;
    emit userNameChanged(m_userName);
}

void IrcServer::setNickName(QString nickName)
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->setUserName(nickName);

    if (m_nickName == nickName)
        return;

    m_nickName = nickName;
    emit nickNameChanged(m_nickName);
}

void IrcServer::setCurrentInstance(IrcServer* currentInstance)
{
    if (m_currentInstance == currentInstance)
        return;

    m_currentInstance = currentInstance;
    emit currentInstanceChanged(m_currentInstance);
}
