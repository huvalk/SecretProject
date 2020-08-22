import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.1
import CustomItems 1.0
import StyleSettings 1.0

DefaultPage {
    defaultFocusItem: _loginInput

    ColumnLayout {
        id: _column
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        spacing: 20

        Label {
            Layout.alignment: Qt.AlignCenter
            text: "<b>Авторизация</b>"
        }

        DefaultInput {
            id: _loginInput
            height: btnHeight
            width: 350

            color: _style.inputColor
            selectionColor: _style.btnSecondaryColor
        }

        DefaultInput {
            id: _pswInput
            height: btnHeight
            width: 350
            echoMode: TextInput.Password

            color: _style.inputColor
            selectionColor: _style.btnSecondaryColor
        }

        DefaultButton {
            // под размер картинки 96*47
            id: _searchFileBtn
            height: btnHeight
            width: btnWidth
            Layout.alignment: Qt.AlignCenter

            text: "Войти"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
//            btnIconSource: Resources.images.browseFileIcon
            btnShadow: _style.primaryOpacity
            btnRadius: _style.btnRadius

            onClicked: {
                backBtnClicked()
            }
        }
    }
}
