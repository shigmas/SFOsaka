import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Row {
    id: headerComponent

    signal addActivated()

    TextField {
        id: textInput
        focus: true
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
