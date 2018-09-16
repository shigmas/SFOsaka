import QtQuick 2.4
import QtQuick.Controls 2.2
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
            Layout.alignment: Qt.AlignTop
            onButtonActivated: root.buttonActivated()
        }

        RowLayout {
            id: searchPhraseLayout
            Layout.alignment: Qt.AlignTop
            height: fontMetrics.height
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
            Layout.alignment: Qt.AlignTop
            width: parent.width
            height: fontMetrics.height

            TextField {
                id: textInput
                width: fontMetrics.width * 2
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
            Layout.alignment: Qt.AlignTop
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
            Layout.alignment: Qt.AlignTop | Qt.AlignBottom
            Layout.fillHeight: true
            width: root.width
            height: root.height
            contentWidth: root.width
            contentHeight: root.height * 2

            ListView {
                id: translationView
                model: translationModel
                //Layout.maximumWidth: fontMetrics.width*3
                width: translationModel.maxCharacters * fontMetrics.width + 4
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
