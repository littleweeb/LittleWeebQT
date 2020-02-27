#include "Headers/QmlControllers/ircchatformcontroller.h"
#include <QDebug>
IrcChatFormController::IrcChatFormController(QObject *parent) : QObject(parent)
{

}

void IrcChatFormController::registerIrcClient(IrcClient *client)
{
    QObject::connect(client, SIGNAL(onNewMessage(QString, QString, QString)),this, SLOT(onNewMessage(QString, QString, QString)));
    QObject::connect(this, SIGNAL(sendMessageToServer(QString)), client, SLOT(sendMessage(QString)));
}


void IrcChatFormController::sendMessage(QString aMessage)
{
    qDebug() << "Send message: " << aMessage;
    emit sendMessageToServer(aMessage);
}

void IrcChatFormController::onNewMessage(QString aMessage, QString aChannel, QString aUser)
{
    emit messageReceived(aChannel, aUser, aMessage);
}
