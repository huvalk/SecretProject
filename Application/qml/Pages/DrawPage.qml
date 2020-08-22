import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.1
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
                _floorsView.updateMapFloors(floors)
            }
        }
    }

    Column {
        id: _toolBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: _backGround.right
        anchors.bottom: parent.bottom
        width: btnWidth
        spacing: _style.defaultSpace

        BaseText {
            id: _cursorLabel
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: "Курсор"
        }

        Row {
            EyeCheckBox {
                id: _isLineAttachment
                trueIndicator: Resources.images.magnetImage
                falseIndicator: Resources.images.unmagnetImage
                onCheckedChanged: {
                    if (checked) {
                        _canvas.setMod(1)
                    } else {
                        _canvas.setMod(0)
                    }
                }
            }
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
                id: _isBackGroundSceneVisable
                trueIndicator: Resources.images.eyeOpened
                falseIndicator: Resources.images.eyeClosed
                onCheckedChanged: {
                    _canvas.setBackgroundVisible(checked)
                }
            }

            EyeCheckBox {
                id: _isBackGroundMoves
                trueIndicator: Resources.images.pinnedImage
                falseIndicator: Resources.images.unpinnedImage
                onCheckedChanged: {
                    if (checked) {
                        _canvas.setMod(2)
                    } else {
                        _canvas.setMod(0)
                    }
                }
            }
        }

        DefaultButton {
            // под размер картинки 96*47
            id: _browseBackgroundImage
            height: btnHeight
            width: btnWidth

            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
//            btnIconSource: Resources.images.openImage
            text: '"Открыть"'
            btnShadow: _style.primaryOpacity
            btnRadius: _style.btnRadius

            onClicked: {
               _fileDialog.open()
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

                trueIndicator: Resources.images.eyeOpened
                falseIndicator: Resources.images.eyeClosed
                onCheckedChanged: {
                    _canvas.setBackgroundFloorVisible(checked)
                }
            }

            DefaultInput {
                id: _otherFloorNumber
                anchors.verticalCenter: parent.verticalCenter
                width: 40
                height: 26

                inputMethodHints: Qt.ImhFormattedNumbersOnly
                color: _style.inputColor
                selectionColor: _style.btnSecondaryColor
                placeholderText: "1"

                onTextChanged: {
                    let floor = parseInt(text)
                    if (!isNaN(floor)) {
                        _canvas.setBackgroundFloor(floor)
                    }
                }
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

    FileDialog {
        id: _fileDialog

        modality: Qt.WindowModal
        visible: false
        title: "Выберете файл с картой"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        nameFilters: [ "Image files (*.jpg *.png *.jpeg)", "All files (*)" ]
        selectedNameFilter: "*"
        sidebarVisible: true

        onAccepted: { _canvas.setBackground(fileUrl )}
        onRejected: { console.log("Rejected") }
    }
}
