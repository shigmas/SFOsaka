import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    id: root
    property string itemImageSource
    property string itemTitle
    property string itemTime
    property string itemDescription

    x: 5
    width: parent.width
    height: parent.width

    Row {
        Text {
            text: root.itemTime
            font.family: "Arial"
            font.pointSize: 12
        }

        Image {
            width: 100
            height: 100
            fillMode: Image.PreserveAspectFit
            source: root.itemImageSource
        }
        ColumnLayout {
            Row {
                Text {
                    text: root.itemTitle
                    font.bold: true
                    font.family: "Arial"
                    font.pointSize: 20
                }
            }
            Row {
                TextArea {
                    text: root.itemDescription
                    font.family: "Arial"
                    font.pointSize: 12
                }
            }
        }
    }
}
