import QtQuick 2.0
import QtQuick.Controls 2.12
import ResourceProvider 1.0

CheckBox {
    id: root

    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: root.leftPadding
        y: parent.height / 2 - height / 2
        radius: _style.btnRadius

        Image {
            width: 14
            height: 14
            x: 6
            y: 6
            visible: root.checked

            source: Resources.images.eyeOpened
        }

        Image {
            width: 14
            height: 14
            x: 6
            y: 6
            visible: !root.checked

            source: Resources.images.eyeClosed
        }

        Rectangle {
            anchors.fill: parent
            color: _style.inputColor
            opacity: root.pressed ? _style.primaryOpacity : root.hovered ? 0 : _style.primaryOpacity * 0.7
        }
    }
}

