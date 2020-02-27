#ifndef IRCCONNECTFORMCONTROLLER_H
#define IRCCONNECTFORMCONTROLLER_H
#include <QObject>
#include <QString>
#include "Headers/IrcClient/ircclient.h"
class IrcConnectFormController : public QObject
{
    Q_OBJECT
public:
    explicit IrcConnectFormController(QObject *parent = nullptr);
    void registerIrcClient(IrcClient *client);
public slots:
    void connectToServerRequest(QString aServer, int aPort, QString aUserName, QString aChannels);
    void connectionStatus(QString aConnectionStatus, bool aStatus, QString aServer, int aPort, QString aUserName, QString aChannels);
signals:
    void connectToServer(QString aServer, int aPort, QString aUserName, QString aChannels);
    void loading(bool aIsLoading);
    void loadingMessageChanged(QString aLoadingMessage);
    void connectedToServer(QString aServer, int aPort, QString aUserName, QString aChannel);
};

#endif // IRCCONNECTFORMCONTROLLER_H
