import QtQuick 2.6
import StyleSettings 1.0

Rectangle {
    property bool selected: false
    signal delegateClicked

    id: root
    radius: _style.btnRadius
    color: "transparent"

    Rectangle {
        anchors.fill: parent
        radius: root.radius

        color: _style.btnSecondaryColor
        opacity: _delegateArea.pressed ?  _style.primaryOpacity : 0
    }

    MouseArea {
        id: _delegateArea
        anchors.fill: root
        onClicked: {
            root.delegateClicked()
        }
    }
}
