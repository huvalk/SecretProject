import QtQuick 2.6
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import ResourceProvider 1.0
import CustomItems 1.0
import StyleSettings 1.0
import SearchList 1.0

DefaultPage {
    property alias openFilePath: _filePathInput.text
    property int openedMapID
    signal createNewClicked
    signal browseFileClicked
    signal editFileClicked
    signal openFileClicked
    defaultFocusItem: _searchView

    id: root

    ColumnLayout {
        id: _column
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: _backBtn.top
        anchors.margins: {
            leftMargin: _style.windowMarginSide
            rightMargin: _style.windowMarginSide
            topMargin: _style.windowMarginBottom
            bottomMargin: _style.windowMarginBottom
        }
        spacing: 8

        ToolSeparator {
            Layout.fillWidth: true
            orientation: Qt.Horizontal
        }

        Label {
            text: "<b>Создать новую карту</b>"
        }
        Row {
            DefaultInput {
                id: _filePathInput
                color: _style.inputColor
                height: btnHeight
                width: _column.width // - _browseFileBtn.width

                selectionColor: _style.btnSecondaryColor
            }

//            DefaultButton {
//                // под размер картинки 96*47
//                id: _browseFileBtn
//                height: btnHeight
//                width: btnWidth/2

//                btnOverlayColor: _style.btnPrimaryColor
//                btnPrimaryColor: _style.btnSecondaryColor
//                btnIconSource: Resources.images.browseFileIcon
//                btnShadow: _style.primaryOpacity
//                btnRadius: _style.btnRadius

//                onClicked: {

//                    root.browseFileClicked()
//                }
//            }
        }

        Row {
            DefaultCheckBox {
                id: _rememberFilePath
                height: btnHeight

                text: "Параметры по умолчанию"

                onCheckStateChanged: {
                    if (checked) {
                        _animateParametrsOpen.start()
                    } else {
                        _animateParametrsClose.start()
                    }

                }
            }

            DefaultButton {
                id: _createNewBtn
                width: btnWidth
                height: btnHeight

                text: "Создать"
                btnOverlayColor: _style.btnPrimaryColor
                btnPrimaryColor: _style.btnSecondaryColor
                btnShadow: _style.primaryOpacity
                btnRadius: _style.btnRadius

                onClicked: {
                    openedMapID = database.saveMapFile(_filePathInput.text)
                    root.createNewClicked()
                }
            }
        }

        ToolSeparator {
            Layout.fillWidth: true
            orientation: Qt.Horizontal
        }

        Label {
            id: _parametrsLabel
            opacity: 0.3
            text: "Параметры"
        }

        ToolSeparator {
            Layout.fillWidth: true
            orientation: Qt.Horizontal
        }

        PropertyAnimation {
            id: _animateParametrsOpen
            target: _parametrsLabel
            properties: "opacity"
            from: 0.3
            to: 1.0
            duration: 500
       }

        PropertyAnimation {
            id: _animateParametrsClose
            target: _parametrsLabel
            properties: "opacity"
            from: 1.0
            to: 0.3
            duration: 500
       }

        Label {
            text: "<b>Поиск карты</b>"
        }

        Row {
            id: _searchRow

            DefaultInput {
                id: _searchInput
                height: btnHeight
                width: _column.width - _searchFileBtn.width

                color: _style.inputColor
                selectionColor: _style.btnSecondaryColor
            }

            DefaultButton {
                // под размер картинки 96*47
                id: _searchFileBtn
                height: btnHeight
                width: btnWidth/2

                btnOverlayColor: _style.btnPrimaryColor
                btnPrimaryColor: _style.btnSecondaryColor
                btnIconSource: Resources.images.browseFileIcon
                btnShadow: _style.primaryOpacity
                btnRadius: _style.btnRadius
            }
        }

        Row {
            id: _listRow
            Layout.fillHeight: true
            spacing: _style.defaultSpace

            Rectangle {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: _column.width - _openFileBtn.width - _style.defaultSpace

                color: _style.inputColor
                radius: _style.btnRadius
                clip: true

                SearchView {
                    id: _searchView
                    anchors.fill: parent
                }
            }

            Column {
                spacing: _style.defaultSpace

                DefaultButton {
                    id: _openFileBtn
                    height: btnHeight
                    width: btnWidth

                    text: "Просмотр"
                    btnOverlayColor: _style.btnPrimaryColor
                    btnPrimaryColor: _style.btnSecondaryColor
                    btnShadow: _style.primaryOpacity
                    btnRadius: _style.btnRadius

                    onClicked: {
                        openedMapID = _searchView.currentMapFileID()
                        root.openFileClicked()
                    }
                }

                DefaultButton {
                    id: _editFileBtn
                    height: btnHeight
                    width: btnWidth

                    text: "Редактор"
                    btnOverlayColor: _style.btnPrimaryColor
                    btnPrimaryColor: _style.btnSecondaryColor
                    btnShadow: _style.primaryOpacity
                    btnRadius: _style.btnRadius

                    onClicked: {
                        openedMapID = _searchView.currentMapFileID()
                        root.editFileClicked()
                    }
                }
            }
        }

    }

    BackButton {
        id: _backBtn
        text: "Выход"
    }

    onBackBtnClicked: {
        _mainWindow.close()
    }
}
