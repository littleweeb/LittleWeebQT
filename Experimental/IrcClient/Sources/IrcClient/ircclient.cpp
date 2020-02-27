#include "Headers/IrcClient/ircclient.h"
#include <QTimer>
#include <QEventLoop>
#include <QTcpSocket>
#include <QTextCodec>
#include <QElapsedTimer>
#include <QDebug>
#include <iostream>

IrcClient::IrcClient(QObject *parent) : QObject(parent)
{
    pConnectionState = Disconnected;
    connect(this, SIGNAL(startRegister(QString)),this, SLOT(Register(QString)));
    connect(this, SIGNAL(startJoin(QString)),this, SLOT(Join(QString)));
}

void IrcClient::registerDccClient(DccClient *dccClient)
{
    connect(this, SIGNAL(onTcpServerMessage(QString)),dccClient, SLOT(parseDccRequest(QString)));
}

void IrcClient::connectToServer(QString aServer, int aPort, QString aUserName, QString aChannels)
{
    pServer = aServer;
    pPort = aPort;
    pUserName = aUserName;
    pChannels = aChannels;
    //@todo add tcp connection here
    //@todo add protocol communication/handshake here
    int retries = 0;
    QEventLoop loop;
    do {
        QTimer::singleShot(100, &loop, SLOT(quit()));
        loop.exec();
        switch(pConnectionState)
        {
            case Disconnected:
                UpdateStatus("Disconnected", false);
                Connect();
                continue;
            case Connecting:
                continue;
            case Connected:
                UpdateStatus("Connected", false);
                qDebug() << "Starting registering procedure!";
                connect(this, SIGNAL(onTcpServerMessage(QString)),this, SLOT(Register(QString)));
                emit startRegister("");
                continue;
            case Registering:
                emit startRegister("");
                continue;
            case Registered:
                UpdateStatus("Registered", false);
                qDebug() << "Starting joining procedure!";
                connect(this, SIGNAL(onTcpServerMessage(QString)),this, SLOT(Join(QString)));
                disconnect(this, SIGNAL(onTcpServerMessage(QString)),this, SLOT(Register(QString)));
                pConnectionState = Joining;
                continue;
            case Joining:
                emit startJoin("");
                continue;
            case Joined:
                disconnect(this, SIGNAL(onTcpServerMessage(QString)),this, SLOT(Join(QString)));
                UpdateStatus("Finished", true);
                break;
            case Failed:
                UpdateStatus("Failed (after retries)", false);
                break;
            case Retry:
                retries++;
                UpdateStatus("Retrying", false);
                closeConnection();
                pConnectionState = Disconnected;
                continue;
        }
        break;
    } while(true);
}

void IrcClient::sendMessage(QString aMessage)
{
   QList<QString> tcpMessage = ircInputValidator.ParseInput(aMessage, pChannels);
   qDebug() << "Sending message to irc server: " << tcpMessage;
   this->pTcpSocket->write(tcpMessage[0].toUtf8());
   this->pTcpSocket->flush();
   emit onNewMessage(aMessage, tcpMessage[1], pUserName);
}

void IrcClient::closeConnection()
{
    qDebug() << "Closing connection to irc server: " << pServer;
    this->pTcpSocket->close();
}

void IrcClient::onTcpConnect()
{
    pConnectionState = Connected;
}

void IrcClient::onTcpDisconnect()
{
    pConnectionState = Disconnected;
    UpdateStatus("Disconnected", false);
}

void IrcClient::onReadyRead()
{
    while (this->pTcpSocket->canReadLine())
    {
       QString data = QString(this->pTcpSocket->readLine());
       qDebug() << "TCP Received: " << data;
       if(ircOutputValidator.IsResponse(data))
       {
           qDebug() << "GOT SERVER RESPONSE: " << ircOutputValidator.ResponseCode(data);
       }
       if(ircOutputValidator.IsError(data))
       {
           qDebug() << "GOT SERVER ERROR: " << ircOutputValidator.ErrorCode(data);
       }
       if(data.contains("PING"))
       {
           QString pingId = data.split(':')[1];
           QString pong = QString("PONG :%1").arg(pingId);
           this->pTcpSocket->write(pong.toUtf8());
           this->pTcpSocket->flush();
       }
       emit onTcpServerMessage(data);

       QString message = QString("");
       QString user = QString("");
       QString channel = QString("");

       if(ircOutputValidator.ParseMessage(data, &channel, &user, &message))
       {
           emit onNewMessage(message, channel, user);
       }
    }
}

void IrcClient::onBytesWritten(qint64 bytes)
{
    qDebug() << "Bytes written:";
    qDebug() << bytes;
}

void IrcClient::Connect()
{
    pConnectionState = Connecting;

    this->pTcpSocket = new QTcpSocket(this);
    connect(this->pTcpSocket, SIGNAL(connected()),this, SLOT(onTcpConnect()));
    connect(this->pTcpSocket, SIGNAL(disconnected()),this, SLOT(onTcpDisconnect()));
    connect(this->pTcpSocket, SIGNAL(bytesWritten(qint64)),this, SLOT(onBytesWritten(qint64)));
    connect(this->pTcpSocket, SIGNAL(readyRead()),this, SLOT(onReadyRead()));

    // this is not blocking call
    this->pTcpSocket->connectToHost(pServer, static_cast<quint16>(pPort));

    // we need to wait...
    if(!this->pTcpSocket->waitForConnected(5000))
    {
        qDebug() << "Error: " << this->pTcpSocket->errorString();
    }
}

void IrcClient::UpdateStatus(QString aConnectionStatus, bool aStatus)
{
     emit onConnectionStatusChange(aConnectionStatus, aStatus, pServer, pPort, pUserName, pChannels);
}

void IrcClient::Register(QString aMessage)
{
    qDebug() << "Received input for REGISTER: " << aMessage;
    enum RegisterStatus {RegPassword, ValPassword, RegUser, ValUser, RegFinished};
    static RegisterStatus registerStatus = RegUser; // SKIPPING PASSWORD @todo implement when necessary
    QString registerString;
    static QElapsedTimer timer;
    switch(registerStatus)
    {
        case RegPassword:
            UpdateStatus("Registering Password.", false);
            registerString = QString("PASS %1 \r\n").arg(""); //@todo store password which we currently do not have.
            this->pTcpSocket->write(registerString.toUtf8());
            this->pTcpSocket->flush();
            registerStatus = ValPassword;
            break;
        case RegUser:
            pConnectionState = Registering;
            UpdateStatus("Registering Nickname.", false);
            registerString = QString("NICK %1\r\n").arg(pUserName);
            this->pTcpSocket->write(registerString.toUtf8());
            this->pTcpSocket->flush();
            UpdateStatus("Registering User.", false);
            registerString = QString("USER %1 8 * :%2_qtIrcClient\r\n").arg(pUserName, pUserName);
            this->pTcpSocket->write(registerString.toUtf8());
            this->pTcpSocket->flush();
            registerStatus = ValUser;
            timer.restart(); //timeout timer
            break;
        case ValPassword:
            pConnectionState = Registering;
            UpdateStatus("Registering Password.", false);
            registerStatus = RegUser;
            break;
        case ValUser:
            UpdateStatus("Validating Registered User.", false);
            qDebug() << "COME ON: " << ircOutputValidator.ResponseCode(aMessage);
            if(ircOutputValidator.ResponseCode(aMessage) == RPL_WELCOME)
            {
                registerStatus = RegFinished;
                timer.restart();
            } else
            {
                qDebug() << "Elapsed time since start registering: " << timer.elapsed();
                if(timer.elapsed() >= timeout)
                {
                    UpdateStatus("Failed registering.", false);
                    pConnectionState = Failed;
                    registerStatus = RegFinished;
                }
            }
            break;
        case RegFinished:
            if(ircOutputValidator.ResponseCode(aMessage) == RPL_LUSERME)
            {
                UpdateStatus("Finished registering.", false);
                pConnectionState = Registered;
            } else if(timer.elapsed() >= timeout)
            {
                UpdateStatus("Failed registering.", false);
                pConnectionState = Failed;
                registerStatus = RegFinished;
            }


            qDebug() << "Finished registering, ignored message";
            break;
    }

}

void IrcClient::Join(QString aMessage)
{
    qDebug() << "Received input for JOIN: " << aMessage;
    enum JoinStatus {SendJoin, ValJoin, FinishedJoin};
    static JoinStatus joinStatus = SendJoin;

    QString registerString;
    static QElapsedTimer timer;
    switch(joinStatus)
    {
        case SendJoin:
            pConnectionState = Joining;
            UpdateStatus(QString("Joining Channel(s) = %1.").arg(pChannels), false);
            registerString = QString("JOIN %1\r\n").arg(pChannels); //@todo store password which we currently do not have.
            this->pTcpSocket->write(registerString.toUtf8());
            this->pTcpSocket->flush();
            this->pTcpSocket->write(registerString.toUtf8());
            this->pTcpSocket->flush();
            timer.restart();
            joinStatus = ValJoin;
            break;
        case ValJoin:
            UpdateStatus(QString("Validating Join Channel(s) = %1.").arg(pChannels), false);
            qDebug() << aMessage;
            if(ircOutputValidator.ResponseCode(aMessage) == RPL_ENDOFNAMES)
            {
                pConnectionState = Joined;
                joinStatus = FinishedJoin;
            }
            else if(ircOutputValidator.ErrorCode(aMessage) == ERR_NOTREGISTERED)
            {
                pConnectionState = Failed;
                joinStatus = FinishedJoin;
            }
            else
            {
                qDebug() << "Time elapsed: " << timer.elapsed() <<  " of: " << (timeout);
                if(timer.elapsed() > timeout)
                {
                    pConnectionState = Failed;
                    joinStatus = FinishedJoin;
                }
            }
            break;
        case FinishedJoin:
            qDebug() << "Finished joining, ignore message: " << aMessage;
            break;
    }
}



