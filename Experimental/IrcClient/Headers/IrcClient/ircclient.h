#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include "ircoutputvalidator.h"
#include "ircinputvalidator.h"
#include "Headers/DccClient/dccclient.h"

class IrcClient : public QObject
{
    Q_OBJECT
public:
    explicit IrcClient(QObject *parent = nullptr);
    void registerDccClient(DccClient *dccClient);
public slots:
    void connectToServer(QString aServer, int aPort, QString aUserName, QString aChannels);
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
    void onNewMessage(QString message, QString channel, QString user);
    void onTcpServerMessage(QString aMessage);
    void startRegister(QString aMessage);
    void startJoin(QString aMessage);
private:
    IrcOutputValidator ircOutputValidator;
    IrcInputValidator ircInputValidator;
    enum ConnectionState {Disconnected, Connecting, Connected, Registering, Registered, Joining, Joined, Failed, Retry};
    ConnectionState pConnectionState;
    QTcpSocket *pTcpSocket;

    const static int retry = 3;
    const static qint64 timeout = 9000;

    void Connect();
    void Disconnect();
    void UpdateStatus(QString aConnectionStatus, bool aStatus);

    QString pServer;
    int pPort;
    QString pUserName;
    QString pChannels;

};

#endif // IRCCLIENT_H
