import QtQuick 2.6


Rectangle {
    property alias text: _input.text
    property alias selectionColor: _input.selectionColor

    clip: true

    TextInput {
        id: _input
        anchors.fill: parent
        anchors.leftMargin: font.pointSize
        anchors.rightMargin: font.pointSize
        verticalAlignment: TextInput.AlignVCenter
        autoScroll: true
        selectByMouse: true
    }
}
