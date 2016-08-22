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
        text: "p"
    }

    ColumnLayout {
        id: row1
        Layout.fillWidth: true
        anchors.fill: parent

        AppBar {
            id: toolbar
            anchors.top: parent.top
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
            id: searchBoxLayout
            anchors.top: searchPhraseLayout.bottom
            width: parent.width

            TextField {
                id: textInput
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

        RowLayout {
            id: resultsExplanationLayout
            anchors.top: searchBoxLayout.bottom
            Rectangle {
                width: 1
                height: 0
            }

            Text {
                id: resultsExplanationText
                font.pointSize: 12
                text: translateController.resultsText
            }
        }

        ScrollView {
            id: scrollView
            anchors.top: resultsExplanationLayout.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            ListView {
                id: translationView
                model: translationModel
                //Layout.maximumWidth: fontMetrics.width*3
                contentWidth: translationModel.maxCharacters * fontMetrics.width + 4
                flickableDirection: Flickable.HorizontalAndVerticalFlick
                delegate: Rectangle {
                    id: itemRect
                    height: fontMetrics.height
                    width: translationModel.maxCharacters * fontMetrics.width
                    Row {
                        Text { id: textLabel; text: word + ": " }
                        Text { id: transLabel; text: translation }
                        Text { id: pronLabel; text: pronunciation }
                    }
                }
            }
        }
    }

}
