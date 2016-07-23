import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Row {
    id: headerComponent
    spacing: 6

    signal addActivated()

    TextMetrics {
        id: fontMetrics
        font.family: "Arial"
        font.pointSize: 20
        text: "translator"
        elideWidth: 100
        elide: Text.ElideMiddle
    }

    TextEdit {
        id: textInput
        width: fontMetrics.width
        height: fontMetrics.height

        objectName: "translatorInput"
        inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
        Keys.onPressed: {
            translateController.OnInputAccepted(text)
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
