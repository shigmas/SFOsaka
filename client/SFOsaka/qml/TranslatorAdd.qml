import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

import SFOsaka 1.0

Item {
    id: root
    anchors.fill: parent
    width: parent.width
    signal buttonActivated()

    ColumnLayout {
        anchors.fill: parent

        AppBar {
            id: toolbar
            anchors.top: parent.top
            width: parent.width
            onButtonActivated: root.buttonActivated()
        }

        TextMetrics {
            id: fontMetrics
            font.pointSize: 12
            text: "placeholder text"
        }
        TextArea {
            id: instructionText
            anchors.top: toolbar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            implicitHeight: fontMetrics.height * 6
            font.pixelSize: 12
            readOnly: true
            text: qsTr("Enter a new phrase in Hiragana and Romanji. Under translation(s), add the translation of the phrase into English, including alternate translations if necessary. Translations will appear when they are approved.")
        }

        ScrollView {
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: instructionText.bottom
            anchors.bottom: parent.bottom

            ColumnLayout {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: toolbar.width

                RowLayout {
                    id: newPhraseLayout
                    Text {
                        id: searchPhraseText
                        text: qsTr("New Phrase")
                    }
                }

                Row {
                    id: column1

                    TextField {
                        id: textInput
                        inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                        text: submitModel.word
                        placeholderText: "ひらがな"
                        //font.pixelSize: 12
                        validator: SFOValidator {
                            identifier: "word"
                            receiver: submitModel
                        }
                    }
                    TextField {
                        id: textPhonetic
                        inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                        text: submitModel.phonetic
                        placeholderText: qsTr("romaji")
                        //font.pixelSize: 12 
                        validator: SFOValidator {
                            identifier: "phonetic"
                            receiver: submitModel
                        }
                    }
                }
                Row {
                    Text {
                        text: qsTr("Translation(s):")
                    }
                }

                ListView {
                    id: listView1
                    //        x: 200
                    //        y: 62
                    width: 110
                    height: 160
                    delegate: Row {
                        TextField {
                            id: rowText
                            inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                            placeholderText: qsTr("English")
                            text: translation
                            width: 120
                            height: 40
                            validator: SFOValidator {
                                identifier: index
                                receiver: submitModel
                            }
                        }
                        Rectangle {
                            width: 5
                        }
                        ToolButton {
                            visible: addVisible
                            width: 32
                            text: qsTr("+")
                            id: addButton
                            onClicked: {
                                submitModel.AddTranslation(rowText.text)
                            }
                        }
                        ToolButton {
                            visible: removeVisible
                            width: 32
                            text: qsTr("-")
                            id: removeButton
                            onClicked: {
                                // index is set from the model
                                submitModel.RemoveTranslation(index)
                            }
                        }
                    }
                    model: submitModel
                }
                Row {
                    Text {
                        id: submitEnabledText
                        font.pointSize: 12
                        text: submitModel.submitEnabledText
                    }
                }
                Row {
                    Button {
                        id: submitButton
                        enabled: submitModel.submitEnabled
                        text: qsTr("Submit")
                    }
                }
            }
        }

        MessageDialog {
            id: postSubmitDialog
            title: qsTr("Thank you for your submission")
            text: qsTr("Your phrase and translation has been submitted. It will appear in the phrasebook after it has been approved.")
            onAccepted: {
                stackView.pop()
            }
        }

        Connections {
            target: submitButton
            onClicked: {
                listView1.model.CommitModel()
                postSubmitDialog.open()
            }
        }
    }
}
