#ifndef IRCCHATFORMCONTROLLER_H
#define IRCCHATFORMCONTROLLER_H

#include <QObject>
#include "Headers/IrcClient/ircclient.h"

class IrcChatFormController : public QObject
{
    Q_OBJECT
public:
    explicit IrcChatFormController(QObject *parent = nullptr);
    void registerIrcClient(IrcClient *client);
public slots:
    void sendMessage(QString aMessage);
    void onNewMessage(QString aMessage, QString aChannel, QString aUser);
signals:
    void sendMessageToServer(QString aMessage);
    void messageReceived(QString aChannel, QString aFrom, QString aMessage);
};

#endif // IRCCHATFORMCONTROLLER_H
