import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    property real xPos
    property real yPos
    property int index 
    property string title
    property string shortDescription

    property real titleFontHeight
    property real titleFontWidth
    id: popup

    TextMetrics {
        id: titleFontMetrics
        font.bold: true
        font.pointSize: 14
        text: title
    }

    TextMetrics {
        id: descFontMetrics
        font.pointSize: 10
        text: shortDescription
    }
    // Some magic numbers here. Enough to make it spacious enough.
    // Width needs to be a minimum, but grow if necessary.
    height: (titleFontMetrics.height + descFontMetrics.height) * 1.6
    width: titleFontMetrics.width * 1.3 > 70 ? titleFontMetrics.width * 1.3 : 70
    // negative is closer to the top of the screen. So, take the marker position
    // and mov it up a little bit, and move it to the left (to center it a
    // little bit).
    x: xPos - width * 0.25
    y: yPos - height + 20

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
                anchors.leftMargin: 5
                font.pointSize: 14
                //horizontalAlignment: Text.AlignHCenter
                //verticalAlignment: Text.AlignVCenter
                font.bold: true
                color: "steelblue"
                text: title
            }
            Text {
                anchors.leftMargin: 5
                //horizontalAlignment: Text.AlignHCenter
                //verticalAlignment: Text.AlignVCenter
                //anchors.bottom: parent.bottom
                font.pointSize: 10
                color: "black"
                text: shortDescription
                width: parent.width
                wrapMode: Text.Wrap
            }
        }
        MouseArea {
            id: imageMouseArea
            anchors.fill: parent
            onPressed: {
                console.log("item selected")
                console.log("w/h: " + width + "/" + height, "pos: (" + x + "," + y + ")")
                itemSelected(title)
            }
        }
    }
}
