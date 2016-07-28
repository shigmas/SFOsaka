import QtQuick 2.4
import QtQuick.Controls 1.4

Item {
    id: root

    signal buttonActivated()

    TextMetrics {
        id: titleFontMetrics
        font.family: "Arial"
        font.bold: true
        font.pointSize: 28
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
        anchors.fill: parent
        
        AppBar {
            id: toolbar
            onButtonActivated: root.buttonActivated()
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
                    font.pointSize: 28
                }
            }
        }

        Row {
            id: row3

            Text {
                id: partnerCategory
                text: partnerDetail.category
                font.pointSize: 12
            }
        }

        Row {
            id: row4

            TextArea {
                id: partnerDescription
                font.pointSize: 14
                text: partnerDetail.description
            }
        }
    }
}
