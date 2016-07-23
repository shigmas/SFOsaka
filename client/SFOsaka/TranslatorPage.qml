import QtQuick 2.4
import QtQuick.Controls 1.4


Item {
    //anchors.fill: parent
    id: root

    signal buttonActivated()
    signal addActivated()

    property alias itemButton: button1

    TextMetrics {
        id: fontMetrics
        font.family: "Arial"
        font.pointSize: 20
        text: "placeholder text"
    }

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
                id: translationView
                model: translationModel
                delegate: Rectangle {
                    width: fontMetrics.width*3
                    height: fontMetrics.height
                    Row {
                        Text { text: word + ": " }
                        Text { text: translation }
                        Text { text: pronunciation }
                    }
                }
            }

        }
    }

}
