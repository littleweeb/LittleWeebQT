import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/components"
import "qrc:/views"


Item{
    id: ircConnectForm
    anchors.fill: parent

    Connections
    {
        target: _ircConnectFormController
        onLoadingMessageChanged:{
            loaderId.text = aLoadingMessage
        }
        onLoading:{
            loaderId.visible = aIsLoading;
            connectionFormColId.visible = !aIsLoading;
        }
        onConnectedToServer:{
            connectionFormColId.visible = false;
            loaderId.opacity = 0
            ircChatFormId.channels = String(aChannel)
            ircChatFormId.server = String(aServer)
            ircChatFormId.visible = true
        }
    }

    Loader
    {
        anchors.centerIn: parent
        id: loaderId
        visible: false
    }

    Column{
        spacing: 10
        id: connectionFormColId
        anchors.centerIn: parent
        FormTextInput
        {
            width: 150
            id: ircServerInputId
            text: "Irc Server (e.g. irc.rizon.net)"
        }
        FormTextInput
        {
            width: 150
            id: ircPortInputId
            text: "Irc Port (e.g. 6667)"
        }
        FormTextInput
        {
            width: 150
            id: ircUserNameInputId
            text: "Irc User Name"
        }
        FormTextInput
        {
            width: 150
            id: ircChannelsInputId
            text: "Irc Channels (eg #channel1,#channel2)"
        }
        Button
        {
            width: 150
            id: connectButtonId
            text: "Connect"
            onClicked: _ircConnectFormController.connectToServerRequest(ircServerInputId.text, ircPortInputId.text, ircUserNameInputId.text, ircChannelsInputId.text);
        }
    }

    IrcChatForm
    {
        id: ircChatFormId
        visible: false
    }
}


