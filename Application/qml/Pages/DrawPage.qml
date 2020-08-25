import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import CustomItems 1.0
import CustomItems.DefaultList 1.0
import MapFloorsView 1.0
import ResourceProvider 1.0
import GraphicScene 1.0

DefaultPage {
    property int mapID
    function uploadMap(mod) {
        // TODO переместитьбазу данных в c++
        if ( _canvas.parseJSONScene( database.getMap(mapID) ) ) {
            _canvas.setMod(mod)
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

            onMapChanged: {
                _toolBar.updateMapFloors(floors)
            }
        }
    }

    ViewToolBar {
        id: _toolBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: _backGround.right
        anchors.bottom: parent.bottom
        width: btnWidth
        spacing: _style.defaultSpace
        canvas: _canvas
    }

    DefaultButton {
        // под размер картинки 96*47
        id: _saveMapBtn
        height: btnHeight
        width: btnWidth
        anchors.horizontalCenter: _toolBar.horizontalCenter
        anchors.bottom: _backBtn.top
        anchors.bottomMargin: _style.defaultSpace

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
        anchors.horizontalCenter: _toolBar.horizontalCenter
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
