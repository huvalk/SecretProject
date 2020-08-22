pragma Singleton
import QtQuick 2.6

Item {
    property alias images : _images
    QtObject {
        id: _images
        readonly property string browseFileIcon: "qrc:/qml/resources/browse_file.png"
        readonly property string searchFileIcon: "qrc:/qml/resources/search_file.png"
        readonly property string defaultUserIcon: "qrc:/qml/resources/user_file.png"
        readonly property string loadImage: "qrc:/qml/resources/load_image_holder.jpeg"
        readonly property string eyeOpened: "qrc:/qml/resources/eye_opened.png"
        readonly property string eyeClosed: "qrc:/qml/resources/eye_closed.png"
        readonly property string openImage: "qrc:/qml/resources/open_image.png"
        readonly property string pinnedImage: "qrc:/qml/resources/pinned.png"
        readonly property string unpinnedImage: "qrc:/qml/resources/unpinned.png"
        readonly property string magnetImage: "qrc:/qml/resources/magnet.png"
        readonly property string unmagnetImage: "qrc:/qml/resources/unmagnet.png"
    }
}
