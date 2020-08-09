import QtQuick 2.6
import CustomItems 1.0
import StyleSettings 1.0

DefaultButton {
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    anchors.margins: {
        leftMargin: _style.windowMarginSide
        bottomMargin: _style.windowMarginBottom
    }
    width: btnWidth
    height: btnHeight

    font.bold: true
    btnPrimaryColor: defBtnColor
    btnRadius: _style.btnRadius

    onClicked: {
        parent.backBtnClicked()
    }
}
