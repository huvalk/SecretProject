import QtQuick 2.6

QtObject {
    readonly property int btnBigWidth: _mainWindow.width * 0.2 + 100
    readonly property int btnBigHeight: _mainWindow.height / 640 * 50
    readonly property int btnSmallWidth: 95
    readonly property int btnSmallHeight: 25
    readonly property int btnRadius: 3
    readonly property int itemRadius: 8
    readonly property int defaultSpace: 8
    readonly property int windowMarginSide: ( ( _mainWindow.height - _mainWindow.height % 960 ) / 960 + 1 ) * 55
    readonly property int windowMarginBottom: _mainWindow.width / 640 * 25
    readonly property color primaryColor: "black"
    readonly property color secondaryColor: "#41cd52"
    readonly property color btnPrimaryColor: "#7fc2ff"
    readonly property color btnSecondaryColor: "#41cd52"
    readonly property color backgroundColor: "lightgrey"
    readonly property color defaultTextColor: "black"
    readonly property color altTextColor: "white"
    readonly property color inputColor: "ghostwhite"
    readonly property int transitionDuration: 300
    readonly property real primaryOpacity: 0.6
}
