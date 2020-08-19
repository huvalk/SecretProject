import QtQuick 2.6
import QtQuick.Controls 2.1
import CustomItems.DefaultList 1.0
import StyleSettings 1.0
import MapFloors 1.0


DefaultListView {
    function updateMapFloors(floors) {
        _model.updateMapFloors(floors)
    }
    function upFloor() {
        currentIndex = _model.upFloor(currentIndex)
    }
    function downFloor() {
        currentIndex = _model.downFloor(currentIndex)
    }
    function getFloor(index) {
        return _model.getFloor(index)
    }

    id: root

    model: MapFloorsModel {
        id: _model
    }

    delegate: MapFloorsDelegate {
        id: _delegate
        width: root.width - 1.5*_style.defaultSpace
        height: 30
        selected: ListView.isCurrentItem
        onDelegateClicked: {
            root.currentIndex = index
        }
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
