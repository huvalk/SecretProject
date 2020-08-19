import QtQuick 2.6

ListView {
    spacing: 1
    section.criteria: ViewSection.FirstCharacter
    section.property: "name"

    highlight: Rectangle {
        color:  _style.btnPrimaryColor
        radius: 5
    }
}
