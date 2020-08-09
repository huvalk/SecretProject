import QtQuick 2.6
import QtQuick.Controls 2.1
import CustomItems.DefaultList 1.0
import StyleSettings 1.0
import MapFiles 1.0


DefaultListView {
    id: root
    model: MapFileModel {

    }

    delegate: SearchDelegate {
        width: root.width - 1.5*_style.defaultSpace
        height: 70
    }

    ScrollBar.vertical: ScrollBar {
        policy: ScrollBar.AsNeeded
        background: Rectangle {
            anchors.fill: parent
            radius: width / 2

            color: "ghostwhite"
        }

        contentItem: Rectangle {
            implicitWidth: 6
            implicitHeight: 100
            radius: width / 2

            color: parent.pressed ? _style.btnPrimaryColor : "#aed8b4"
            opacity: parent.active ? 1 : _style.primaryOpacity
        }
    }
}
