import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    property int index 
    property string popupTitle
    property string popupSubTitle

    property real titleFontHeight
    property real titleFontWidth

    TextMetrics {
        id: titleFontMetrics
        font.bold: true
        font.pointSize: 14
        text: popupTitle
    }

    TextMetrics {
        id: descFontMetrics
        font.pointSize: 10
        text: popupSubTitle
    }
    // Some magic numbers here. Enough to make it spacious enough.
    // Width needs to be a minimum, but grow if necessary.
    height: (titleFontMetrics.height + descFontMetrics.height) * 1.6
    width: titleFontMetrics.width * 1.3 > 70 ? titleFontMetrics.width * 1.3 : 120
    // negative is closer to the top of the screen. So, take the marker position
    // and mov it up a little bit, and move it to the left (to center it a
    // little bit).

    titleFontHeight: titleFontMetrics.height
    titleFontWidth: titleFontMetrics.width
    
    Rectangle {
        id: textRect
        anchors.fill: parent
        border.width: 1
        border.color: "black"
        radius: 6.0

        ColumnLayout {
            Text {
                font.pointSize: 14
                font.bold: true
                color: "steelblue"
                leftPadding: 4.0
                text: popupTitle
            }
            Text {
                font.pointSize: 10
                color: "black"
                Layout.maximumWidth: parent.width
                Layout.minimumWidth: parent.width
                width: parent.width
                horizontalAlignment: Text.AlignLeft
                text: popupSubTitle
                leftPadding: 4.0
                wrapMode: Text.Wrap
            }
        }
        MouseArea {
            id: imageMouseArea
            anchors.fill: parent
            onPressed: {
                console.log("item selected")
                itemSelected(index)
            }
        }
    }
}
