import QtQuick 2.0
import GraphicScene 1.0

GraphicScene {
    id: _canvas
    // позиционируем его и задаём размеры
    anchors.top: parent.top
    anchors.topMargin: 50
    anchors.horizontalCenter: parent.horizontalCenter
    width: 200
    height: 200

    // Определяем его свойства, которые Q_PROPERTY
    name: "clock"
    backgroundColor: "white"

    Rectangle {
        id: _backGround
        anchors.fill: parent
        color: "white"
    }
}
