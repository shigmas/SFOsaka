import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: root
    property string itemTitle
    property string itemImageURL
    property string itemDescription
    property string itemDetail

    width: parent.width
    //height: parent.width
    implicitHeight: contentItem.height
    
    TextMetrics {
        id: infoFont
        font.family: "Arial"
        font.pointSize: 12
    }

    TextMetrics {
        id: titleFont
        font.family: "Arial"
        font.pointSize: 22
        text: root.itemTitle
    }

    // Use one item to contain the other items, so we can use its height in
    // the parent
    ColumnLayout {
        id: contentItem
        // I think using the parent as anchors.fill is weird, since its
        // implicit height depends on us, so use the Layout fill
        Layout.fillHeight: true
        Layout.fillWidth: true
        width: parent.width
        spacing: 5

        // This has a bar so that it divides each entry
        Rectangle {
            id: titleItem
            color: "lightgray"
            Layout.fillWidth: true
            width: parent.width
            height: titleFont.height
            Text {
                text: root.itemTitle
                anchors.leftMargin: 8.0
                font.family: "Arial"
                font.pointSize: 22
                font.bold: true
            }
        }

        GridLayout {
            id: gridlayout
            columns: 2
            flow: GridLayout.LeftToRight
            Layout.fillWidth: true
            width: parent.width
            Layout.alignment: Qt.AlignTop
            ColumnLayout {
                id: infoColumn
                Layout.alignment: Qt.AlignLeft
                anchors.leftMargin: 4.0
                anchors.rightMargin: 4.0

                Text {
                    id: detail
                    text: root.itemDetail
                    font.family: "Arial"
                    font.pointSize: 14
                }
            }

            Rectangle {
                id: imageRectItem
                width: parent.width - infoColumn.width
                height: parent.width - infoColumn.width
                Layout.alignment: Qt.AlignRight
                anchors.bottomMargin: 6.0
                Image {
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    source: root.itemImageURL
                }
            }

            Text {
                id: partnerDescription
                // Handles if we don't have an image at all, to make sure the
                // description doesn't scrunch up over the info.
                topPadding: 3.0
                anchors.top: infoColumn.height > imageRectItem.height ? infoColumn.bottom : imageRectItem.bottom
                Layout.fillWidth: true
                Layout.columnSpan: 2
                // Colspan 2, so double, since I guess this value is adjusted
                // for our place in the layout?
                Layout.maximumWidth: parent.width * 2
                width: parent.width
                font.pointSize: 14
                wrapMode: Text.Wrap
                text: root.itemDescription
            }
        }
            
    }
}
