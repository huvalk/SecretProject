import QtQuick 2.6
import QtQuick.Controls 2.12


Rectangle {
    property alias text: _input.text
    property alias selectionColor: _input.selectionColor
    property alias echoMode: _input.echoMode
    property alias inputMethodHints: _input.inputMethodHints
    property alias placeholderText: _input.placeholderText

    clip: true

    TextField {
        id: _input
        anchors.fill: parent
//        anchors.leftMargin: font.pointSize
//        anchors.rightMargin: font.pointSize
        verticalAlignment: TextInput.AlignVCenter
        autoScroll: true
        selectByMouse: true
    }
}
