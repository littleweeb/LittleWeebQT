import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import "qrc:/components"


Item{
    id: ircChatForm
    anchors.fill: parent
    property var channels: String("Dummy Channel")
    property var server:  String("Dummy Server");
    property var userName: String("Dummy user");;

    Connections
    {
        target: _ircChatFormController
        onMessageReceived:
        {
            print(aChannel);
            print(aFrom);
            print(aMessage);
            chatHistoryListModelId.append({"channel": String(aChannel), "from": String(aFrom), "message":String(aMessage) });
            chatViewId.model = chatHistoryListModelId;
        }
    }

    DccDownloadAccepter
    {
        height: parent.height
        width: parent.width
        id: dccDownloadAccepterId
    }

    ListModel
    {
        id: chatHistoryListModelId
        ListElement
        {
            property var channel: "Test Channel";
            property var from: "Test From";
            property var message: "Test message";
        }
    }
    Column {
        height: parent.height
        width: parent.width
        Flickable
        {
            id: chatViewFlickableId
            height: parent.height - ircChatInputId.height
            width: parent.width
            contentHeight: chatViewColumnId.height
            ScrollBar.vertical: ScrollBar { }
            clip: true
            Column {
                id: chatViewColumnId
                width: parent.width
                Repeater {
                    id: chatViewId
                    model: chatHistoryListModelId
                    width: parent.width
                    Row{
                        spacing: 5
                        Text { text: "Channel: " + channel }
                        Text { text: " From: " + from }
                        Text { text: " Message: " + message }
                    }
                }
            }
            onContentHeightChanged: {
                contentY = chatViewColumnId.height - height
                print("Content view height: " )
                print(chatViewColumnId.height - chatViewId.height)
            }
        }

        Row{
            spacing: 10
            id: ircChatInputId
            FormTextInput
            {
                width: 500
                id: chatInputId
                text: "Test"
            }

            Button
            {
                id: connectButtonId
                width: 140
                text: "Send"
                onClicked: _ircChatFormController.sendMessage(chatInputId.text, ircChatForm.channels)
            }
        }
    }
}


