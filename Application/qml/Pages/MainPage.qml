import QtQuick 2.6
import QtQuick.Controls 2.12
import CustomItems 1.0
import StyleSettings 1.0

DefaultPage {
    property int btnMainWidth: 50
    property int btnMainHeight: 50
    defaultFocusItem: _createBtn

    Column {
        anchors.centerIn: parent
        spacing: 20

        DefaultButton {
            id: _createBtn
            width: btnMainWidth
            height: btnMainHeight

            text: "Создать"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor

            onClicked: {
                _stackView.push(_openPage)
            }
        }

        DefaultButton {
            id: _openBtn
            width: btnMainWidth
            height: btnMainHeight

            text: "Открыть"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor

            onClicked: {
                _stackView.push(_openPage)
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
