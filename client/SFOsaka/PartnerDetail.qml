import QtQuick 2.4
import QtQuick.Controls 1.4

Item {
    id: root

    signal buttonActivated()

    property alias itemButton: button1

    TextMetrics {
        id: titleFontMetrics
        font.family: "Arial"
        font.bold: true
        font.pointSize: 42
        text: "placeholder text"
    }

    TextMetrics {
        id: textFontMetrics
        font.family: "Arial"
        font.pointSize: 20
        text: "placeholder text"
    }

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

            Rectangle {
                width: titleFontMetrics.width*3
                height: titleFontMetrics.height

                Text {
                    id: partnerName
                    text: partnerDetail.name
                    font.family: "Arial"
                    font.bold: true
                    font.pointSize: 42
                }
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
