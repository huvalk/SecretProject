import QtQuick 2.6
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import ResourceProvider 1.0
import CustomItems 1.0
import StyleSettings 1.0
import SearchList 1.0

DefaultPage {
    property alias openFilePath: _filePathInput.text
    signal createNewClicked
    signal browseFileClicked
    defaultFocusItem: _browseFileBtn

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
            text: "<b>Загрузить с диска</b>"
        }
        Row {
            DefaultInput {
                id: _filePathInput
                color: _style.inputColor
                height: btnHeight
                width: _column.width - _browseFileBtn.width

                selectionColor: _style.btnSecondaryColor
            }

            DefaultButton {
                // под размер картинки 96*47
                id: _browseFileBtn
                height: btnHeight
                width: btnWidth/2

                btnOverlayColor: _style.btnPrimaryColor
                btnPrimaryColor: _style.btnSecondaryColor
                btnIconSource: Resources.images.browseFileIcon
                btnShadow: _style.primaryOpacity
                btnRadius: _style.btnRadius

                onClicked: {
                    root.browseFileClicked()
                }
            }
        }

        Row {
            DefaultCheckBox {
                id: _rememberFilePath
                height: btnHeight

                text: "Запомнить выбор"
            }

            DefaultButton {
                id: _createNewBtn
                width: btnWidth
                height: btnHeight

                text: "Открыть"
                btnOverlayColor: _style.btnPrimaryColor
                btnPrimaryColor: _style.btnSecondaryColor
                btnShadow: _style.primaryOpacity
                btnRadius: _style.btnRadius

                onClicked: {
                    root.createNewClicked()
                }
            }
        }

        ToolSeparator {
            Layout.fillWidth: true
            orientation: Qt.Horizontal
        }

        Label {
            text: "<b>Поиск в сети</b>"
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
                btnIconSource: Resources.images.searchFileIcon
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
                width: _column.width - _downloadFileBtn.width - _style.defaultSpace

                color: _style.inputColor
                radius: _style.btnRadius
                clip: true

                SearchView {
                    anchors.fill: parent
                }
            }

            DefaultButton {
                id: _downloadFileBtn
                height: btnHeight
                width: btnWidth

                text: "Скачать"
                btnOverlayColor: _style.btnPrimaryColor
                btnPrimaryColor: _style.btnSecondaryColor
                btnShadow: _style.primaryOpacity
                btnRadius: _style.btnRadius
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
