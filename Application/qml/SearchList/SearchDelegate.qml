import QtQuick 2.6
import CustomItems.DefaultList 1.0
import CustomItems 1.0
import StyleSettings 1.0
import ResourceProvider 1.0
import QtGraphicalEffects 1.0

DefaultListDelegate {
    property int mapFileID: mapID
    id: root

    ImageBubble {
        id: _contactBubble
        anchors.left: root.left
        anchors.verticalCenter: root.verticalCenter
        anchors.leftMargin: _style.defaultSpace
        boundingRadius: root.height - 15

        image.source: Resources.images.defaultUserIcon
    }

    Column {
        anchors.left: _contactBubble.right
        anchors.leftMargin: _contactBubble.width/2
        anchors.verticalCenter: root.verticalCenter

        spacing: 4
        BaseText {
            text: name
        }
        BaseText {
            text: tag
        }
    }
}
