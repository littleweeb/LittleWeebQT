#ifndef IRCINPUTVALIDATOR_H
#define IRCINPUTVALIDATOR_H

#include <QObject>

class IrcInputValidator : public QObject
{
    Q_OBJECT
public:
    explicit IrcInputValidator(QObject *parent = nullptr);

    //Returns string list where index 0 is the actual tcp message, and 1 the destination if it has any
    QList<QString> ParseInput(QString aInput, QString aTo = "");

private:
    // List with strings needed for translating user input to tcp messages that are irc compliant
    // Format "regex for parsing", "message to be generated"
    // Regex should always use groups, where the first group should ALWAYS be the specified action (prefixed by a '/')
    // The following groups should be used to fill in the missing arguments in the "message to be generated" 0 = group 1, {1} = group 2 et cetera.
    QList<QList<QString>> MessageTranslations
    {
        QList<QString> {"((?<=/join\\s)[^\\s]+)","JOIN 0\r\n"},
        QList<QString> {"((?<=/msg\\s)[^\\s]+)\\s(.*)", "PRIVMSG 0 :1\r\n"},
        QList<QString> {"((?<=/names\\s)[^\\s]+)", "NAMES 0\r\n"},
        QList<QString> {"((?<=/quit\\s)[^\\s]+)", "QUIT 0\r\n"},
        QList<QString> {"((?<=/quit\\s))", "QUIT\r\n"}
    };

signals:
};

#endif // IRCINPUTVALIDATOR_H
