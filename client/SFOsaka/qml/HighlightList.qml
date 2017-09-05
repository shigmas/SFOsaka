import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

Item {
    id: root

    signal buttonActivated()

    TextMetrics {
        id: descriptionFontMetrics
        font.family: "Arial"
        font.pointSize: 20
        text: "word"
    }

    ColumnLayout {
        anchors.fill: parent
        Layout.fillWidth: true
        AppBar {
            id: toolbar
            onButtonActivated: root.buttonActivated()
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: listView1
                //x: 0
                //y: 0
                width: parent.width
                layoutDirection: Qt.RightToLeft
                //width: parent.width
                //height: 1200
                //layoutDirerction: Qt.RightToLeft

                delegate:  HighlightItem {
                    itemTitle: title
                    itemImageURL: imageSource
                    itemDescription: description
                    itemDetail: detail
                }
                model: appHighlightsModel
            }
        }
    }
}

