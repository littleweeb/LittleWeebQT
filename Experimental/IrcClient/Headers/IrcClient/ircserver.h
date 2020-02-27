#ifndef IRCSERVER_H
#define IRCSERVER_H
#include <QObject>
class IrcServer : public QObject {
    Q_PROPERTY(QString serverIp READ getServerIp WRITE setServerIp NOTIFY serverIpChanged)
    Q_PROPERTY(QString serverPort READ getServerPort WRITE setServerPort NOTIFY serverPortChanged)
    Q_PROPERTY(bool useSsl READ getUseSsl WRITE setUseSsl NOTIFY useSslChanged)
    Q_PROPERTY(QList<QString> channels READ getChannels WRITE setChannels NOTIFY channelsChanged)
    Q_PROPERTY(QString userName READ getUserName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString nickName READ getNickName WRITE setNickName NOTIFY nickNameChanged)
    Q_PROPERTY(IrcServer* currentInstance READ getCurrentInstance WRITE setCurrentInstance NOTIFY currentInstanceChanged)


public:
    QString getServerIp() const;
    QString getServerPort() const;
    bool getUseSsl() const;
    QList<QString> getChannels() const;
    QString getUserName() const;
    QString getNickName() const;
    IrcServer* getCurrentInstance() const;

public slots:
    void setServerIp(QString serverIp);
    void setServerPort(QString serverPort);
    void setUseSsl(bool useSsl);
    void setChannels(QList<QString> channels);
    void setUserName(QString userName);
    void setNickName(QString nickName);
    void setCurrentInstance(IrcServer* currentInstance);

signals:
    void serverIpChanged(QString serverIp);
    void serverPortChanged(QString serverPort);
    void useSslChanged(bool useSsl);
    void channelsChanged(QList<QString> channels);
    void userNameChanged(QString userName);
    void nickNameChanged(QString nickName);
    void currentInstanceChanged(IrcServer* currentInstance);

private:
    QString m_serverIp;
    QString m_serverPort;
    bool m_useSsl;
    QList<QString> m_channels;
    QString m_userName;
    QString m_nickName;
    IrcServer* m_currentInstance;
};

#endif // IRCSERVER_H
