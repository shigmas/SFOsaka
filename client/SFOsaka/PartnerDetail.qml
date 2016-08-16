import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: root
    width: parent.width

    signal buttonActivated()

    ColumnLayout {
        id: row1
        anchors.fill: parent
        width: parent.width
        
        AppBar {
            id: toolbar
            anchors.top: parent.top
            onButtonActivated: root.buttonActivated()
        }

        GridLayout {
            id: gridlayout
            columns: 2
            flow: GridLayout.LeftToRight
            Layout.fillWidth: true
            width: parent.width
            anchors.top: toolbar.bottom
            anchors.bottom: parent.bottom

            ColumnLayout {
                id: infoColumn
                anchors.leftMargin: 4.0
                anchors.rightMargin: 4.0

                Text {
                    id: partnerName
                    text: partnerDetail.name
                    anchors.leftMargin: 4.0
                    anchors.top: parent.top
                    font.family: "Arial"
                    font.bold: true
                    font.pointSize: 20
                }
                Text {
                    id: partnerCategory
                    text: partnerDetail.category
                    font.family: "Arial"
                    font.bold: true
                    font.pointSize: 16
                }
                Text {
                    id: partnerStreet
                    text: partnerDetail.contactStreet
                    font.family: "Arial"
                    font.pointSize: 16
                }
                Text {
                    id: partnerCity
                    text: partnerDetail.contactCity
                    font.family: "Arial"
                    font.pointSize: 16
                }
            }

            Rectangle {
                id: imageRectItem
                width: parent.width - infoColumn.width
                height: parent.width - infoColumn.width
                anchors.bottomMargin: 6.0
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    source: partnerDetail.imageURL
                }
            }

            Text {
                id: partnerDescription
                topPadding: 3.0
                anchors.top: imageRectItem.bottom
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.maximumWidth: parent.width * 2
                width: parent.width * 2
                font.pointSize: 14
                wrapMode: Text.Wrap
                text: partnerDetail.description
            }

        }
    }
}
