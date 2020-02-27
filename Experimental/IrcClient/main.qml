import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "qrc:/views"
import "qrc:/components"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Drawer {
       id: drawer
       width: 0.66 * window.width
       height: window.height

       Label {
           text: "Content goes here!"
           anchors.centerIn: parent
       }
   }

    IrcConnectForm
    {
        anchors.fill: parent
    }

    IrcChatForm
    {
        id: ircChatFormId
        visible: false
    }


}
