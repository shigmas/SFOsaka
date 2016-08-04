import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import SFOsaka 1.0

Item {
    id: root
    anchors.fill: parent
    signal buttonActivated()

    ColumnLayout {
        width: parent.width

        AppBar {
            id: toolbar
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
            height: fontMetrics.height * 3
            font.pixelSize: 12
            readOnly: true
            text: qsTr("Enter a new phrase in either English or Japanese. For Japanese phrases, include Romaji. Under translation(s), add the translation of the phrase in the destination language, including alternate translations if necessary. Translations will appear when they are approved.")
        }

        RowLayout {
            id: newPhraseLayout
            anchors.top: instructionText
            Text {
                id: searchPhraseText
                text: qsTr("New Phrase")
            }
        }

        Row {
            id: column1
            anchors.top: newPhraseLayout.bottom

            TextField {
                id: textInput
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                text: submitModel.word
                placeholderText: qsTr("Japanese/English")
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
                    placeholderText: qsTr("English/Japanese")
                    text: translation
                    width: 120
                    height: 40
                    validator: SFOValidator {
                        identifier: [index, "translation"]
                        receiver: submitModel
                    }
                }
                TextField {
                    id: rowPhonetic
                    inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                    placeholderText: qsTr("romaji")
                    text: phonetic
                    width: 120
                    height: 40
                    validator: SFOValidator {
                        identifier: [index, "phonetic"]
                        receiver: submitModel
                    }
                }
                ToolButton {
                    visible: addVisible
                    width: 32
                    text: qsTr("+")
                    id: addButton
                    onClicked: {
                        submitModel.AddTranslation(rowText.text,
                                                   rowPhonetic.text)
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
            Button {
                id: submitButton
                text: qsTr("Submit")
            }
        }

        Connections {
            target: submitButton
            onClicked: {
                listView1.model.CommitModel()
                stackView.pop()
            }
        }
    }
}
