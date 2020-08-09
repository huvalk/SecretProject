import QtQuick 2.6
import StyleSettings 1.0
import QtGraphicalEffects 1.0

Rectangle {
    id: root
    property alias image: _internalImage

    property real boundingRadius
    width: boundingRadius
    height: boundingRadius

    color: "transparent"
    radius: width / 2
    border.width: 2
    border.color: _style.primaryColor

    RoundImage {
        id: _internalImage
        anchors.fill: root
        anchors.margins: 0
        sourceSize.height: height
        sourceSize.width: width
    }
}
