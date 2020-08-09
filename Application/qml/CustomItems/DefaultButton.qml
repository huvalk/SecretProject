import QtQuick 2.6
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

Button {
    property string btnPrimaryColor: ""
    property string btnOverlayColor: ""
    property string btnIconSource: ""
    property real btnShadow: 0.3
    property int btnRadius: 0

    id: root

    background: Rectangle {
        radius: btnRadius
        color: btnPrimaryColor

        Rectangle {
            anchors.fill: parent
            color: btnOverlayColor
            antialiasing: true

            opacity: root.pressed ? btnShadow : root.hovered ? 0 : btnShadow * 0.7
        }
    }

    Image {
        anchors.fill: parent
        source: btnIconSource
    }
}
