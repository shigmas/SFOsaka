import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

ColumnLayout {
    id: root

    signal buttonActivated()

    Button {
        id: button1
        anchors.top: parent.top
        anchors.left: parent.left
        text: qsTr("go back")
        onClicked: root.buttonActivated()
    }

    Column {
        Text {
            text: qsTr("Placeholder text")
        }
    }
}
