import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Row {
    id: headerComponent

    signal addActivated()

    TextMetrics {
        id: fontMetrics
        font.family: "Arial"
        font.pointSize: 20
        text: "translator"
        elideWidth: 100
        elide: Text.ElideMiddle
    }

    Rectangle {
        border.width: 3
        width: fontMetrics.width
        height: fontMetrics.height

        TextEdit {
            id: textInput
            objectName: "translatorInput"
            inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
            focus: true
            Keys.onPressed: {
                translateController.OnInputAccepted(text)
            }
        }
    }
    Button {
        id: button1
        text: qsTr("+")
        onClicked: {
            headerComponent.addActivated()
        }
    }
}
