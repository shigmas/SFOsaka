import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import SFOsaka 1.0

Item {
    //anchors.fill: parent
    id: root

    signal buttonActivated()
    signal addActivated()

    TextMetrics {
        id: fontMetrics
        font.family: "Arial"
        font.pointSize: 20
        text: "placeholder text"
    }

    ColumnLayout {
        id: row1
        Layout.fillWidth: true
        anchors.fill: parent

        AppBar {
            id: toolbar
            onButtonActivated: root.buttonActivated()
        }

        RowLayout {
            id: searchPhraseLayout
            anchors.top: toolbar.bottom
            Rectangle {
                width: 1
                height: 0
            }

            Text {
                id: searchPhraseText
                text: qsTr("Search Phrase")
            }
        }

        RowLayout {
            id: header
            anchors.top: searchPhraseLayout.bottom

            TextField {
                id: textInput
                //Layout.fillWidth: true
                width: fontMetrics.width * 2
                height: fontMetrics.height
                placeholderText: qsTr("English/Japanese")
                validator: SFOValidator {
                    receiver: translateController
                }
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                id: button1
                text: qsTr("Add")
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    root.addActivated()
                }
            }
        }

        ListView {
            id: translationView
            model: translationModel
            anchors.top: header.bottom
            anchors.bottom: parent.bottom
            
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
