import QtQuick 2.6
import QtQuick.Controls 2.12

StackView {
    property alias durationPushEnter: animationPushEnter.duration
    property alias durationPushExit: animationPushExit.duration
    property alias durationPopEnter: animationPopEnter.duration
    property alias durationPopExit: animationPopExit.duration

    pushEnter: Transition {
        PropertyAnimation {
            id: animationPushEnter
            property: "x"
            from: - _mainWindow.width
            to: 0
        }
    }

    pushExit: Transition {
        PropertyAnimation {
            id: animationPushExit
            property: "x"
            from: 0
            to: _mainWindow.width
        }
    }

    popEnter: Transition {
        PropertyAnimation {
            id: animationPopEnter
            property: "x"
            from: _mainWindow.width
            to: 0
        }
    }

    popExit: Transition {
        PropertyAnimation {
            id: animationPopExit
            property: "x"
            from: 0
            to: - _mainWindow.width
        }
    }
}
