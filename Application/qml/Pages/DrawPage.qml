import QtQuick 2.0
import QtQuick.Controls 2.12
import CustomItems 1.0
import CustomItems.DefaultList 1.0
import MapFloorsView 1.0
import GraphicScene 1.0

DefaultPage {
    property int mapID
    function uploadMap() {
        // TODO переместитьбазу данных в c++
        if ( _canvas.parseJSONScene( database.getMap(mapID) ) ) {

        }
    }

    defaultFocusItem: _canvas

    Rectangle {
        id: _backGround
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width - _saveMapBtn.width
        color: "white"
        focus: true

        GraphicScene {
            id: _canvas
            anchors.fill: parent
            focus: true
            backgroundColor: "whiteSmoke"

            onMapChanged: {
                _floorsView.updateMapFloors(floors)
            }
        }
    }

    Item {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: btnWidth

        DefaultButton {
            // под размер картинки 96*47
            id: _upFloor
            height: btnHeight
            width: btnWidth
            anchors.top: parent.top

            text: "Выше"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
    //        btnIconSource: Resources.images.browseFileIcon
            btnShadow: _style.primaryOpacity
            btnRadius: 0

            onClicked: {
                _floorsView.upFloor()
            }
        }

        DefaultButton {
            // под размер картинки 96*47
            id: _downFloor
            height: btnHeight
            width: btnWidth
            anchors.top: _upFloor.bottom

            text: "Ниже"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
    //        btnIconSource: Resources.images.browseFileIcon
            btnShadow: _style.primaryOpacity
            btnRadius: 0

            onClicked: {
                _floorsView.downFloor()
            }
        }

        MapFloorsView {
            id: _floorsView
            anchors.top: _downFloor.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: _backBtn.top

            onCurrentIndexChanged: {
                _canvas.setFloor(getFloor(currentIndex))
            }
        }

        DefaultButton {
            // под размер картинки 96*47
            id: _saveMapBtn
            height: btnHeight
            width: btnWidth
            anchors.bottom: _backBtn.top

            text: "Сохранить"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
    //        btnIconSource: Resources.images.browseFileIcon
            btnShadow: _style.primaryOpacity
            btnRadius: _style.btnRadius

            onClicked: {
               var a = _canvas.generateJSONScene();
                database.saveMap(mapID, a);
            }
        }

        DefaultButton {
            // под размер картинки 96*47
            id: _backBtn
            height: btnHeight
            width: btnWidth
            anchors.bottom: parent.bottom

            text: "Назад"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
    //        btnIconSource: Resources.images.browseFileIcon
            btnShadow: _style.primaryOpacity
            btnRadius: _style.btnRadius

            onClicked: {
               backBtnClicked()
            }
        }
    }
}
