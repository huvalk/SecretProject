import QtQuick 2.0
import GraphicScene 1.0

DefaultPage {
    defaultFocusItem: _canvas

    Rectangle {
        id: _backGround
        anchors.fill: parent
        color: "white"
        focus: true

        GraphicScene {
            id: _canvas
            // позиционируем его и задаём размеры
            anchors.fill: parent
            focus: true

            // Определяем его свойства, которые Q_PROPERTY
            name: "clock"
            backgroundColor: "whiteSmoke"
        }
    }
}
