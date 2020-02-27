#ifndef IRCMESSAGE_H
#define IRCMESSAGE_H

#include <QObject>
class IrcMessage : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString server READ getServer WRITE setServer NOTIFY serverChanged)
    Q_PROPERTY(QString channel READ getChannel WRITE setChannel NOTIFY channelNotified)
    Q_PROPERTY(QString user READ getUser WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString message READ getMessage WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QString raw_message READ getRawMessage WRITE setRawMessage NOTIFY rawMessageChanged)

public:
    QString getServer() const;
    QString getChannel() const;
    QString getMessage() const;
    QString getRawMessage() const;
    QString getUser() const;
    IrcMessage* getCurrentInstance() const;


public slots:
    void setServer(QString server);
    void setChannel(QString channel);
    void setMessage(QString message);
    void setRawMessage(QString raw_message);
    void setUser(QString user);
    void setCurrentInstance(IrcMessage* instance);

private:
    IrcMessage* m_currentInstance = nullptr;
    QString m_server;
    QString m_channel;
    QString m_message;
    QString m_raw_message;
    QString m_user;
};

#endif // IRCMESSAGE_H
