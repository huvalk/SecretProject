import QtQuick 2.0
import CustomItems 1.0
import MapFloorsView 1.0

Column {
    function mapChanged(floors) {
        _floorsView.updateMapFloors(floors)
    }

    BaseText {
        id: _backGroundSceneLabel
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "Шаблон"
    }

    Row {
        EyeCheckBox {
            id: _backGroundSceneVisable
        }

        DefaultButton {
            // под размер картинки 96*47
            id: _browseBackgroundImage
            anchors.verticalCenter: parent.verticalCenter
            width: 26
            height: 26

            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
            btnIconSource: Resources.images.openImage
            btnShadow: _style.primaryOpacity
            btnRadius: _style.btnRadius

            onClicked: {
               _fileDialog.open()
            }
        }
    }

    BaseText {
        id: _otherFloorLabel
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "Просветить"
    }

    Row {
        EyeCheckBox {
            id: _otherFloorVisable
        }

        DefaultInput {
            id: _otherFloorNumber
            anchors.verticalCenter: parent.verticalCenter
            width: 40
            height: 26

            inputMethodHints: Qt.ImhFormattedNumbersOnly
            color: _style.inputColor
            selectionColor: _style.btnSecondaryColor
        }
    }

    DefaultButton {
        // под размер картинки 96*47
        id: _upFloor
        height: btnHeight
        width: btnWidth

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
        width: parent.width
        height: 300
        clip: true

        onCurrentIndexChanged: {
            _canvas.setFloor(getFloor(currentIndex))
        }
    }

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

    DefaultButton {
        // под размер картинки 96*47
        id: _backBtn
        height: btnHeight
        width: btnWidth

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

