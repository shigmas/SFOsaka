import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Item {
    id: root

    signal mapActivated()
    signal translatorActivated()
    signal infoActivated()


    Column {
        id: column1
        anchors.fill: parent
        anchors.topMargin: 10
        spacing: 5

        Button {
            id: mapButton
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Map of Partners and Events")
            onClicked: root.mapActivated()
        }

        Button {
            id: kansaiButton
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Kansai Ben Phrases")
            onClicked: root.translatorActivated()
        }

        Button {
            id: sisterCityButton
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Sister City Info")
            onClicked: root.infoActivated()
        }
    }


}
