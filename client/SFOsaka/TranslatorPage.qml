import QtQuick 2.4
import QtQuick.Controls 1.4


Item {
    //anchors.fill: parent
    id: root

    signal buttonActivated()
    signal addActivated()

    property alias itemButton: button1

    Column {
        id: row1
        Row {
            id: toprow
            height: 80
            anchors.left: parent.left
            anchors.right: parent.right
            Button {
                id: button1
                text: qsTr("go back")
                onClicked: root.buttonActivated()
            }

            TranslatorHeader {
                id: header

                onAddActivated: {
                    root.addActivated()
                }
            }
        }

        Rectangle {
            anchors.top: toprow.bottom

            id: row2
            ListView {
                width: 100
                height: 125
                id: translationView
                model: translationModel
                delegate: Rectangle {
                    height: 25
                    width: 100
                    Text { text: modelData }
                }
            }

        }
    }

}
