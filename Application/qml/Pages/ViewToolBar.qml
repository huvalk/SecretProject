import QtQuick 2.0
import QtQuick.Dialogs 1.2
import CustomItems 1.0
import ResourceProvider 1.0
import MapFloorsView 1.0

Column {
    property Item canvas
    function updateMapFloors(floors) {
        _floorsView.updateMapFloors(floors)
    }

    id: root

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
                    canvas.setMod(1)
                } else {
                    canvas.setMod(0)
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
                canvas.setBackgroundVisible(checked)
            }
        }

        EyeCheckBox {
            id: _isBackGroundMoves
            trueIndicator: Resources.images.pinnedImage
            falseIndicator: Resources.images.unpinnedImage
            onCheckedChanged: {
                if (checked) {
                    canvas.setMod(2)
                } else {
                    canvas.setMod(0)
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
        text: "Открыть"
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
                canvas.setBackgroundFloorVisible(checked)
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
                    canvas.setBackgroundFloor(floor)
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
            canvas.setFloor(getFloor(currentIndex))
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

        onAccepted: { canvas.setBackground(fileUrl )}
        onRejected: { console.log("Rejected") }
    }
}
