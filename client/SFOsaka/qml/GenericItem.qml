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

        ColumnLayout {
            id: itemLayout
            Layout.fillWidth: true
            width: parent.width
            Layout.alignment: Qt.AlignTop
            // anchors.leftMargin: 4.0
            // anchors.rightMargin: 4.0

            Text {
                id: detail
                text: root.itemDetail
                font.family: "Arial"
                font.pointSize: 14
            }

            Image {
                id: imageItem
                Layout.maximumWidth: root.width
                Layout.minimumWidth: root.width
                Layout.maximumHeight: root.width / sourceSize.width * sourceSize.height
                Layout.alignment: Qt.AlignLeft
                //anchors.bottomMargin: 6.0
                fillMode: Image.PreserveAspectFit
                source: root.itemImageURL
            }

            Text {
                id: partnerDescription
                // Handles if we don't have an image at all, to make sure the
                // description doesn't scrunch up over the info.
                topPadding: 3.0
                Layout.fillWidth: true
                Layout.maximumWidth: parent.width
                font.pointSize: 14
                wrapMode: Text.Wrap
                text: root.itemDescription
            }
        }
            
    }
}
