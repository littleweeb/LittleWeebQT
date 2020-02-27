#include "Headers/IrcClient/ircoutputvalidator.h"
#include <QRegularExpression>
#include <QMetaEnum>
#include <QDebug>


IrcOutputValidator::IrcOutputValidator()
{

}

int IrcOutputValidator::ParseInput(QString aInput)
{
    QRegularExpression re("(?<=\\s)[0-9]{3}(?<!\\s)");
    QRegularExpressionMatch match = re.match(aInput);
    qDebug() << "parsing aInput: " << aInput;
    if (match.hasMatch()) {
        qDebug() << "parsed aInput: " << aInput << " found: " <<  match.captured(0);
        return match.captured(0).toInt();
    }
    return -1;
}

bool IrcOutputValidator::IsError(QString aInput)
{
    int code = ParseInput(aInput);

    if(ErrorCodes.contains(code))
    {
        return true;
    }
    return false;
}

bool IrcOutputValidator::IsResponse(QString aInput)
{
    int code = ParseInput(aInput);

    if(ResponseCodes.contains(code))
    {
        return true;
    }
    return false;
}

int IrcOutputValidator::ResponseCode(QString aInput)
{
    int code = ParseInput(aInput);

    if(IsResponse(aInput))
    {
        return code;
    }
    return -1;
}

int IrcOutputValidator::ErrorCode(QString aInput)
{
    int code = ParseInput(aInput);

    if(IsError(aInput))
    {
        return code;
    }
    return -1;
}

bool IrcOutputValidator::ParseMessage(QString aInput, QString *oChannel, QString *oUser, QString *oMessage)
{
    QRegularExpression re(":(?P<user>[^\\s]+)!(?P<userhost>[^\\s]+)\\sPRIVMSG\\s(?P<channel>[^\\s]+)\\s:(?P<message>[^\\s]+)");
    qDebug() << "Parsing a messsage: " << aInput;
    QRegularExpressionMatch match = re.match(aInput);
    if (match.hasMatch()) {
        qDebug() << "Found a messsage: " << match.captured("message") << " from user: " << match.captured("user");
        *oUser = match.captured("user");
        *oChannel = match.captured("channel");
        *oMessage = match.captured("message");
        qDebug() << "Received from channel: " << *oChannel << " from user: " << *oUser << " mesage: " << *oMessage;
        return true;
    }
    return false;
}
