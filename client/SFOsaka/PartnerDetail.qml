import QtQuick 2.4
import QtQuick.Controls 1.4

Item {
    id: root

    signal buttonActivated()

    property alias itemButton: button1

    Column {
        id: row1
        Row {
            id: toprow
            height: 80
            anchors.left: parent.left
            anchors.right: parent.right
            Button {
                id: button1
                text: qsTr("go back")
                onClicked: root.buttonActivated()
            }

        }

        Row {
            id: row2

            Text {
                id: partnerName
                text: partnerDetail.name
                font.pixelSize: 12
            }
        }

        Row {
            id: row3

            Text {
                id: partnerCategory
                text: partnerDetail.category
                font.pixelSize: 12
            }
        }

        Row {
            id: row4

            TextArea {
                id: partnerDescription
                text: partnerDetail.description
            }
        }
    }
}
