import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.1
import StyleSettings 1.0
import CustomItems 1.0
import SearchList 1.0
import Pages 1.0

Window {
    id: _mainWindow
    visible: true
    minimumWidth: 640
    minimumHeight: 480
    color: _style.backgroundColor

    Style {
        id: _style
    }

    SlideStack {
        id: _stackView
        anchors.fill: parent
        focus: true

        durationPushEnter: _style.transitionDuration
        durationPushExit: _style.transitionDuration
        durationPopEnter: _style.transitionDuration
        durationPopExit: _style.transitionDuration
        onCurrentItemChanged: {
            console.log("item changed")
            if (currentItem) {
              currentItem.defaultFocusItem.focus = true
            }
        }

        initialItem: _mainPage
    }

    MainPage {
        id: _mainPage
        btnMainHeight: _style.btnBigHeight
        btnMainWidth: _style.btnBigWidth
        btnHeight: _style.btnSmallHeight
        btnWidth: _style.btnSmallWidth

        defBtnColor: _style.btnPrimaryColor
        altBtnColor: _style.btnSecondaryColor
    }

    OpenPage {
        id: _openPage
        btnHeight: _style.btnSmallHeight
        btnWidth: _style.btnSmallWidth

        visible: false
        defBtnColor: _style.btnPrimaryColor
        altBtnColor: _style.btnSecondaryColor
        openFilePath: _fileDialog.fileUrl

        onCreateNewClicked: {
            _stackView.push(_drawPage)
        }

        onBrowseFileClicked: {
            _fileDialog.open()
        }
    }

    DrawPage {
        id: _drawPage
        btnHeight: _style.btnSmallHeight
        btnWidth: _style.btnSmallWidth
        focus: true

        visible: false
        defBtnColor: _style.btnPrimaryColor
        altBtnColor: _style.btnSecondaryColor
    }

    FileDialog {
        id: _fileDialog

        modality: Qt.WindowModal
        visible: false
        title: "Выберете файл с картой"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        nameFilters: [ "Data files (*dat)" ]
        selectedNameFilter: "Data file(*dat)"
        sidebarVisible: true

        onAccepted: { console.log("Accepted")  }
        onRejected: { console.log("Rejected") }
    }
}