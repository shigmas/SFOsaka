import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: root

    signal festivalActivated()
    signal mapActivated()
    signal translatorActivated()
    signal infoActivated()


    Grid {
        id: layout
        anchors.rightMargin: 4
        anchors.leftMargin: 5
        anchors.bottomMargin: 5
        anchors.topMargin: 5
        anchors.fill: parent
        columns: 2
        rows: 2
        spacing: 4

        // I'd really like to make this a type, but the onClicked type is
        // unknown to me right now.
        FeatureRect {
            id: festivalApp
            imageSource: "sister_banner.png"
            titleText: qsTr("Festival Info")
            onClicked: root.festivalActivated()
        }
        FeatureRect {
            id: partnerApp
            imageSource: "sfosaka-map-flatten.png"
            titleText: qsTr("Partners")
            onClicked: root.mapActivated()
        }
        FeatureRect {
            id: phrasebookApp
            imageSource: "dictionary.png"
            titleText: qsTr("Kansai Ben Phrases")
            onClicked: root.translatorActivated()
        }
        FeatureRect {
            id: sisterCityApp
            imageSource: "sfosaka_logo.png"
            titleText: qsTr("Sister City Info")
            onClicked: root.infoActivated()
        }
    }

    RowLayout {
        id: bottomInfo
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        ToolButton {
            id: refreshButton
            text: qsTr("Refresh Data")
            font.pointSize: 12
            // placeModel is a weird place to have a refresh, but there's no
            // central object or controller that's accessible via QML.
            onClicked: placeModel.HandleRefresh()
        }

        Rectangle {
            width: 60
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
