#include "Headers/IrcClient/ircmessage.h"

QString IrcMessage::getServer() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getServer();
    return m_server;
}

QString IrcMessage::getChannel() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getChannel();
    return m_channel;
}

QString IrcMessage::getMessage() const
{
    if(m_currentInstance != nullptr)
        return m_currentInstance->getMessage();
    return m_message;
}

QString IrcMessage::getRawMessage() const
{
    if (m_currentInstance != nullptr)
        return m_currentInstance->getRawMessage();
    return m_raw_message;
}

QString IrcMessage::getUser() const
{
    if (m_currentInstance != nullptr)
        return m_currentInstance->getUser();
    return m_user;
}

IrcMessage *IrcMessage::getCurrentInstance() const
{
    return m_currentInstance;
}

void IrcMessage::setServer(QString server)
{
    if(m_currentInstance != nullptr)
    {
        return m_currentInstance->setServer(server);
    }
    if (m_server == server)
        return;

    m_server = server;
}

void IrcMessage::setChannel(QString channel)
{
    if (m_channel == channel)
        return;

    m_channel = channel;
}

void IrcMessage::setMessage(QString message)
{
    if(m_currentInstance != nullptr)
    {
        return m_currentInstance->setMessage(server);
    }
    if (m_message == message)
        return;

    m_message = message;
}

void IrcMessage::setRawMessage(QString raw_message)
{
    if(m_currentInstance != nullptr)
    {
        return m_currentInstance->setRawMessage(raw_message);
    }
    if (m_raw_message == raw_message)
        return;

    m_raw_message = raw_message;
}

void IrcMessage::setUser(QString user)
{
    if(m_currentInstance != nullptr)
    {
        return m_currentInstance->setUser(user);
    }
    if (m_user == user)
        return;

    m_user = user;
}

void IrcMessage::setCurrentInstance(IrcMessage *instance)
{
    if (m_currentInstance == instance)
        return;
    m_currentInstance = instance;
}

