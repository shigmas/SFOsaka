import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

ToolBar {
    id: root
    signal buttonActivated()
    Layout.fillWidth: true

    RowLayout {
        anchors.fill: parent
        ToolButton {
            text: qsTr("Go Back")
            onClicked: root.buttonActivated()
        }
        Rectangle { Layout.fillWidth: true }
    }
}
