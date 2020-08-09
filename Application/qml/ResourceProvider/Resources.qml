pragma Singleton
import QtQuick 2.6

Item {
    property alias images : _images
    QtObject {
        id: _images
        readonly property string browseFileIcon: "qrc:/qml/resources/browse_file.png"
        readonly property string searchFileIcon: "qrc:/qml/resources/search_file.png"
        readonly property string defaultUserIcon: "qrc:/qml/resources/user_file.png"
    }
}
