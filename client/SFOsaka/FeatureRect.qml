import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3


Rectangle {
    property string imageSource
    property string titleText

    signal clicked()

    id: featureRect
    width: parent.width * 0.5
    height: parent.width * 0.5

    Image {
        id: featureImage
        anchors.fill: parent
        source: imageSource
        fillMode: Image.Stretch
    }
    Text {
        id: featureText
        font.italic: true
        color: "steelblue"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: titleText
    }
    MouseArea {
        id: featureArea
        anchors.fill: parent
        onClicked: featureRect.clicked()
    }
}
