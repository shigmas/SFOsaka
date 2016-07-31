import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3


Rectangle {
    property string imageSource
    property string titleText
    property string titleColor: "darkgrey"
    property string styleColor: "lightgrey"
    property string fontFamily: "Avenir SE"
    signal clicked()

    id: featureRect
    width: parent.width * 0.5
    height: parent.width * 0.5
    
    Image {
        id: featureImage
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.9
        height: parent.height*0.9
        source: imageSource
        fillMode: Image.Stretch
    }

    Text {
        id: featureText
        font.italic: true
        font.family: featureRect.fontFamily
        style: Text.Outline
        styleColor: featureRect.styleColor
        color: featureRect.titleColor
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: titleText
    }
    MouseArea {
        id: featureArea
        anchors.fill: parent
        onClicked: featureRect.clicked()
    }
}
