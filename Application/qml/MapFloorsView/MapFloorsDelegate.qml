import QtQuick 2.6
import CustomItems.DefaultList 1.0
import CustomItems 1.0
import StyleSettings 1.0
import ResourceProvider 1.0
import QtGraphicalEffects 1.0

DefaultListDelegate {
    property int mapFileID: mapID
    id: root

    BaseText {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: floor
    }
}
