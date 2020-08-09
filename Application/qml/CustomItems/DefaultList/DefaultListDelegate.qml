import QtQuick 2.6
import StyleSettings 1.0

Rectangle {
    id: root
    radius: _style.btnRadius
    color: _style.btnSecondaryColor

    Rectangle {
        anchors.fill: parent
        radius: root.radius

        color: _style.btnPrimaryColor
        opacity: _delegateArea.pressed ? _style.primaryOpacity : 0
    }

    MouseArea {
        id: _delegateArea
        anchors.fill: root
    }
}
