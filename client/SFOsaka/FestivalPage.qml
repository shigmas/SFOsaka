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

    Column {
        Button {
            id: button1
            //anchors.top: parent.top
            //anchors.left: parent.left
            text: qsTr("go back")
            onClicked: root.buttonActivated()
        }

        ScrollView {
            //anchors.fill: parent
            height: 900

            ListView {
                id: listView1
                //x: 0
                //y: 0
                width: parent.width
                layoutDirection: Qt.RightToLeft
                //width: parent.width
                //height: 1200
                //layoutDirection: Qt.RightToLeft

                delegate:  ListItem {
                    itemImageSource: imageSource
                    itemTitle: title
                    itemTime: time
                    itemDescription: description
                }
                model: festival_schedule
            }
        }
    }
}

