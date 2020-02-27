#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <QObject>
class IrcMessage : public QObject {

    Q_PROPERTY(QString channels READ getChannels WRITE setDownloadId NOTIFY downloadIdChanged)
    Q_PROPERTY(QString server READ getServerIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(QString origin READ getFileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString destination READ getFileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString origin READ getFileName WRITE setFileName NOTIFY fileNameChanged)



};

#endif // IRCMESSAGE_H
