import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: root
    property string itemImageSource
    property string itemTitle
    property string itemTime
    property string itemDescription

    width: parent.width
    //height: parent.width
    implicitHeight: contentItem.height
    
    TextMetrics {
        id: timeFont
        font.family: "Arial"
        font.pointSize: 18
        text: "text"
    }

    // Use one item to contain the other items, so we can use its height in
    // the parent
    ColumnLayout {
        id: contentItem
        // I think using the parent as anchors.fill is weird, since its
        // implicit height depends on us, so use the Layout fill
        Layout.fillHeight: true
        //Layout.fillWidth: true
        width: root.width
        spacing: 5

        // This has a bar so that it divides each entry
        Rectangle {
            id: timeItem
            color: "lightgray"
            Layout.fillWidth: true
            width: root.width
            height: timeFont.height
            Text {
                text: root.itemTime
                anchors.leftMargin: 8.0
                font.family: "Arial"
                font.pointSize: 18
                font.bold: true
            }
        }

        Text {
            id: titleItem
            width: root.width
            anchors.leftMargin: 8.0
            text: root.itemTitle
            font.bold: true
            font.family: "Arial"
            font.pointSize: 22
        }

        Rectangle {
            id: imageRectItem
            width: root.width 
            height: root.width
            Image {
                id: imageItem
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                source: root.itemImageSource
            }
        }

        Text {
            id: descriptionItem
            topPadding: 3.0
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.maximumWidth: parent.width
            width: parent.width
            anchors.topMargin: 6.0
            wrapMode: Text.Wrap
            font.family: "Arial"
            font.pointSize: 16
            text: root.itemDescription
        }

    }
}
