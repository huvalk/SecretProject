import QtQuick 2.0
import QtQuick.Controls 2.12
import CustomItems 1.0
import GraphicScene 1.0

DefaultPage {
    property int mapID
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

            name: "clock"
            backgroundColor: "whiteSmoke"
        }
    }

    Column {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: btnWidth

        DefaultButton {
            // под размер картинки 96*47
            id: _saveMapBtn
            height: btnHeight
            width: btnWidth

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
    }
}
