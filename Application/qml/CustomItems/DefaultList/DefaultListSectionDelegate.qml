import QtQuick 2.6
import StyleSettings 1.0

Rectangle {
    width: container.width
    height: childrenRect.height
    color: "white"

    Text {
        color: _style.textColor
        text: section
        font.bold: true
        font.pixelSize: 20
    }
}
