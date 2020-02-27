#include "Headers/QmlControllers/ircconnectformcontroller.h"
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
#include <iostream>

IrcConnectFormController::IrcConnectFormController(QObject *parent) : QObject(parent)
{

}

void IrcConnectFormController::registerIrcClient(IrcClient *client)
{
    //Connect controllers to services(signals & slots)
    QObject::connect(this, SIGNAL(connectToServer(QString, int, QString, QString)), client, SLOT(connectToServer(QString, int, QString, QString)));
    QObject::connect(client, SIGNAL(onConnectionStatusChange(QString, bool, QString, int, QString, QString)), this, SLOT(connectionStatus(QString, bool, QString, int, QString, QString)));
}

void IrcConnectFormController::connectToServerRequest(QString aServer, int aPort, QString aUserName, QString aChannels)
{
    emit loadingMessageChanged("Trying to connnect to IRC Server!");
    emit connectToServer(aServer, aPort, aUserName, aChannels);
    emit loading(true);
}

void IrcConnectFormController::connectionStatus(QString aConnectionStatus, bool aStatus, QString aServer, int aPort, QString aUserName, QString aChannels)
{
    emit loadingMessageChanged(aConnectionStatus);
    if (aStatus)
    {
        emit loading(false);
        if(aConnectionStatus == "Finished")
        {
            emit connectedToServer(aServer, aPort, aUserName, aChannels);
        }
    }
    else
        emit loading(true);
}
