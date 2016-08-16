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
    signal partnersActivated()

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

    Text {
        id: festivalDescription
        anchors.top: bannerImage.bottom
        Layout.maximumWidth: parent.width
        width: parent.width
        height: fontMetrics.height*3.5
        wrapMode: Text.WrapAnywhere
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
            imageSource: "resources/taro_orange.png"
            titleText: qsTr("Taste of Osaka")
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
            id: partnersApp
            imageSource: "resources/partners_icon.png"
            titleText: qsTr("Partners")
            onClicked: root.partnersActivated()
        }
    }
    }
    RowLayout {
        id: bottomInfo
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Text {
            font.pointSize: 8
            text: "v 1.3"
            anchors.left: parent.left
            MouseArea {
                id: versionMouseArea
                anchors.fill: parent
                onPressed: {
                    globalController.HandleRefresh()
                }
            }
        }
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

