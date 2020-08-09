import QtQuick 2.6
import StyleSettings 1.0

Item {
    property Item defaultFocusItem
    property string defBtnColor: "black"
    property string altBtnColor: "black"
    property int btnWidth: 50
    property int btnHeight: 50
    signal backBtnClicked
}
