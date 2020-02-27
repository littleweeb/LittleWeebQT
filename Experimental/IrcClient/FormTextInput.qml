import QtQuick 2.12
import QtQuick.Controls 2.12


Item
{
    property alias text: textInputId.text
    width: 100
    height: textInputRectId.height
    Rectangle{
        id: textInputRectId
        width: parent.width;
        height: textInputId.implicitHeight + 8;
        border.width: 1
        border.color: "black"
        TextInput
        {
            id: textInputId
            wrapMode: TextInput.Wrap
            anchors.margins: 4
            anchors.fill: parent
            onFocusChanged:{
               if(focus)
                   selectAll()
            }
        }
    }
}

