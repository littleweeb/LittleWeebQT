import QtQuick 2.12
import QtQuick.Controls 2.12

Item
{
    property alias text: loaderTextId.text
    BusyIndicator
    {
        id: busyIndicatorId
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Text
    {
        anchors.top: busyIndicatorId.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        id: loaderTextId
        text: "Loading"
    }
}
