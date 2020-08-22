import QtQuick 2.0
import CustomItems 1.0
import ResourceProvider 1.0

DefaultPage {
    property bool authorized: false
    signal authRequired
    id: root

    defaultFocusItem: _loadImage

    Image {

        id: _loadImage
        anchors.centerIn: parent

        source: Resources.images.loadImage

        onActiveFocusChanged: {
            if (focus === true) {
                _loadTimer.start()
                // Проверка авторизации
                root.authorized = true
            }
        }

        Timer {
            id: _loadTimer
            running: false
            repeat: false
            interval: 2000

            onTriggered: {
                if (_placeHolder.pressed) {
                    authRequired()
                } else {
                    backBtnClicked()
                }
            }
        }

        DefaultButton {
            // под размер картинки 96*47
            id: _placeHolder
            height: btnHeight
            width: btnWidth
            anchors.horizontalCenter: parent.horizontalCenter

            text: "Войти"
            btnOverlayColor: _style.btnPrimaryColor
            btnPrimaryColor: _style.btnSecondaryColor
    //            btnIconSource: Resources.images.browseFileIcon
            btnShadow: _style.primaryOpacity
            btnRadius: _style.btnRadius
        }

    }
}
