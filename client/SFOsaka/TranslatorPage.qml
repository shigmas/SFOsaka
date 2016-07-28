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
            id: header
            anchors.top: toolbar.bottom

            TextField {
                id: textInput
                //Layout.fillWidth: true
                width: fontMetrics.width * 2
                height: fontMetrics.height
                placeholderText: qsTr("search phrase")
                validator: SFOValidator {
                    receiver: translateController
                }
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
            }

            Item { Layout.fillWidth: true }

            ToolButton {
                id: button1
                text: qsTr("+")
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
