import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: root
    anchors.fill: parent

    signal festivalActivated()
    signal mapActivated()
    signal translatorActivated()
    signal infoActivated()

    TextMetrics {
        id: fontMetrics
        font.family: "Avenir"
        font.pointSize: 16
        text: "placeholder text"
    }

    Image {
        id: bannerImage
        anchors.top: parent.top
        height: parent.width*.483
        width: parent.width
        source: "resources/matsuri_banner.png"
        // fillMode: Image.PreserveAspectFit
        fillMode: Image.Stretch
    }

    TextArea {
        id: festivalDescription
        anchors.top: bannerImage.bottom
        width: parent.width
        height: fontMetrics.height*3.5
        readOnly: true
        font.pointSize: 14
        font.family: "Avenir"
        text: qsTr("August 27, 2016 celebration of the 59th year of San Francisco and Osaka's sister city relationship by highlighting the food, culture, and the ties our community has with Osaka.")
    }

    ScrollView {
        anchors.top: festivalDescription.bottom
        anchors.bottom: bottomInfo.top
        anchors.topMargin: 4.0
        width: root.width
        Layout.fillHeight: true
        Layout.fillWidth: true

    Grid {
        id: layout
        width: root.width
        height: root.width
        columns: 2
        rows: 2
        spacing: 4

        // I'd really like to make this a type, but the onClicked type is
        // unknown to me right now.
        FeatureRect {
            id: festivalApp
            imageSource: "resources/festival_icon.png"
            titleText: qsTr("Festival Info")
            onClicked: root.festivalActivated()
        }
        FeatureRect {
            id: partnerApp
            imageSource: "resources/partner_icon.png"
            titleText: qsTr("Partners")
            // Since the icon is mostly white/red, we override the white
            onClicked: root.mapActivated()
        }
        FeatureRect {
            id: phrasebookApp
            imageSource: "resources/dictionary_icon.png"
            titleText: qsTr("Kansai Ben Phrases")
            onClicked: root.translatorActivated() 
        }
        FeatureRect {
            id: sisterCityApp
            imageSource: "resources/sfosaka_logo.png"
            titleText: qsTr("Sister City Info")
            onClicked: root.infoActivated()
        }
    }
    }
    RowLayout {
        id: bottomInfo
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Rectangle {
            width: 40
            height: 0
        }

        Text {
            font.pointSize: 8
            text: server
        }
        Rectangle {
            width: 15
            height: 0
        }
        Text {
            font.pointSize: 8
            text: "2016 Futomen Networks"
        }
    }
}

