#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "ircoutputvalidator.h"
#include "ircinputvalidator.h"
#include "ircmessage.h"
#include "Headers/DccClient/dccclient.h"

class IrcClient : public QObject
{
    Q_OBJECT
public:
    explicit IrcClient(QObject *parent = nullptr);
    void registerDccClient(DccClient *dccClient);
public slots:
    void connectToServer(QString aServer, int aPort, QString aUserName, QString aChannels);
    void setMessageBufferSize(qint32 size);
    void clearMessageBuffer();
    void sendMessage(QString aMessage);
    void closeConnection();
    void onTcpConnect();
    void onTcpDisconnect();
    void onBytesWritten(qint64 bytes);
    void onReadyRead();
    void Register(QString aMessage);
    void Join(QString aMessage);
signals:
    void onConnectionStatusChange(QString aConnectionStatus, bool aStatus, QString aServer, int aPort, QString aUserName, QString aChannels);
    void onNewMessage(IrcMessage* aIrcMessage);
    void onTcpServerMessage(QString aMessage);
    void startRegister(QString aMessage);
    void startJoin(QString aMessage);
    void messageBufferChanged();
private:
    IrcOutputValidator ircOutputValidator;
    IrcInputValidator ircInputValidator;
    enum ConnectionState {Disconnected, Connecting, Connected, Registering, Registered, Joining, Joined, Failed, Retry};
    ConnectionState pConnectionState;
    QTcpSocket *pTcpSocket;

    const int retry = 3;
    const qint64 timeout = 9000;

    QList<IrcMessage*> messagesBuffer;


    void Connect();
    void Disconnect();
    void UpdateStatus(QString aConnectionStatus, bool aStatus);

    QString pServer;
    int pPort;
    QString pUserName;
    QString pChannels;

};

#endif // IRCCLIENT_H
