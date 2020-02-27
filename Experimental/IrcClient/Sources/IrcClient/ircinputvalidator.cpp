#include "Headers/IrcClient/ircinputvalidator.h"
#include <QRegularExpression>
#include <QDebug>

IrcInputValidator::IrcInputValidator(QObject *parent) : QObject(parent)
{

}

QList<QString> IrcInputValidator::ParseInput(QString aInput, QString aTo)
{
    for(int i = 0; i < MessageTranslations.count(); i++)
    {
        QList<QString> toParseWith = MessageTranslations[i];
        QRegularExpression re(toParseWith[0]);
        QRegularExpressionMatch match = re.match(aInput);

        if (match.hasMatch() && re.captureCount() > -1)
        {
            QString tcpMessage = toParseWith[1];
            qDebug() << "Message: " << aInput << " Contains " << re.captureCount() << " capture groups";
            QString to = match.captured(1);
            //make sure to is actually parsed, if not just send the plain tcp message
            //and set to to aTo as that is the actuall destination
            if(to.length() > 1)
            {
                qDebug() << "We actually have a to to parss " << tcpMessage;
                for(int j = 1; j <= re.captureCount(); j++)
                {                   
                   qDebug() << "Replacing in string " << tcpMessage << ": " << QString("%0").arg(j - 1) << " with " << match.captured(j);
                   tcpMessage.replace(QString("%0").arg(j - 1), match.captured(j));
                }
            } else {
                to = aTo;
            }
            return QList<QString> {tcpMessage, to};
        }
    }

    QString tcpMessage = MessageTranslations[1][1].replace("0", aTo).replace("1", aInput);

    return QList<QString> {tcpMessage, aTo};
}
