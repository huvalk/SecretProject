import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import StyleSettings 1.0
import CustomItems 1.0
import SearchList 1.0
import Pages 1.0

Window {
    id: _mainWindow
    visible: true
    minimumWidth: 800
    minimumHeight: 600
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
            if (currentItem) {
              currentItem.defaultFocusItem.focus = true
            }
        }

        initialItem: _loadPage
    }

    LoadPage {
        id: _loadPage
        visible: false

        onBackBtnClicked: {
            _stackView.push(_openPage)
        }

        onAuthRequired: {
            _stackView.push(_authPage)
        }
    }

    AuthPage {
        id: _authPage
        visible: false

        onBackBtnClicked: {
            _stackView.push(_openPage)
        }
    }

//    MainPage {
//        id: _mainPage
//        btnMainHeight: _style.btnBigHeight
//        btnMainWidth: _style.btnBigWidth
//        btnHeight: _style.btnSmallHeight
//        btnWidth: _style.btnSmallWidth

//        defBtnColor: _style.btnPrimaryColor
//        altBtnColor: _style.btnSecondaryColor
//    }

    OpenPage {
        id: _openPage
        btnHeight: _style.btnSmallHeight
        btnWidth: _style.btnSmallWidth

        visible: false
        defBtnColor: _style.btnPrimaryColor
        altBtnColor: _style.btnSecondaryColor

        onCreateNewClicked: {
            _drawPage.mapID = _openPage.openedMapID
            _drawPage.uploadMap()
            _stackView.push(_drawPage)
            _mainWindow.visibility = Window.Maximized
        }

        onOpenFileClicked: {
            _drawPage.mapID = _openPage.openedMapID
            _drawPage.uploadMap()
            _stackView.push(_drawPage)
            _mainWindow.visibility = Window.Maximized
        }

        onBrowseFileClicked: {
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

        onBackBtnClicked: {
            _stackView.pop()
        }
    }
}
