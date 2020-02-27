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
            text: "irc.rizon.net"
        }
        FormTextInput
        {
            width: 150
            id: ircPortInputId
            text: "6667"
        }
        FormTextInput
        {
            width: 150
            id: ircUserNameInputId
            text: "mrcommon"
        }
        FormTextInput
        {
            width: 150
            id: ircChannelsInputId
            text: "#nibl,#testchannel"
        }
        Button
        {
            width: 150
            id: connectButtonId
            text: "Connect"
            onClicked: _ircConnectFormController.connectToServerRequest(ircServerInputId.text, ircPortInputId.text, ircUserNameInputId.text, ircChannelsInputId.text);
        }
    }
}


