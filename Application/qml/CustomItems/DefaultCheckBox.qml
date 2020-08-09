import QtQuick 2.6
import QtQuick.Controls 2.12
import StyleSettings 1.0

CheckBox {
    id: root

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: root.leftPadding
        y: parent.height / 2 - height / 2
        radius: _style.btnRadius

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: _style.btnRadius
            color: root.checked ? _style.btnSecondaryColor : "white"
            visible: root.checked
        }

        Rectangle {
            anchors.fill: parent
            color: _style.inputColor
            opacity: root.pressed ? _style.primaryOpacity : root.hovered ? 0 : _style.primaryOpacity * 0.7
        }
    }

    contentItem: Text {
        text: root.text
        font: root.font
        verticalAlignment: Text.AlignVCenter
        leftPadding: root.indicator.width + root.spacing
    }
}
