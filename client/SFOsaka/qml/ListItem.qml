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
    implicitHeight: contentItem.height

    TextMetrics {
        id: timeFont
        font.family: "Arial"
        font.pointSize: 18
        text: "text"
    }

    TextMetrics {
        id: titleFont
        font.bold: true
        font.family: "Arial"
        font.pointSize: 22
        text: "Text"
    }

    // Use one item to contain the other items, so we can use its height in
    // the parent
    ColumnLayout {
        id: contentItem
        Layout.fillHeight: true
        Layout.fillWidth: true
        width: parent.width
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
            height: titleFont.height
            anchors.leftMargin: 8.0
            text: root.itemTitle
            font.bold: true
            font.family: "Arial"
            font.pointSize: 22
        }

        Image {
            id: imageItem
            Layout.alignment: Qt.AlignTop
            // Expand, but go no further than the width of the item.
            Layout.maximumWidth: root.width
            // Given the width of the item, limit our height to keep the aspect
            // ratio of the image.
            Layout.maximumHeight: root.width / sourceSize.width * sourceSize.height
            fillMode: Image.PreserveAspectFit
            source: root.itemImageSource
        }

        Text {
            id: descriptionItem
            topPadding: 3.0
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
