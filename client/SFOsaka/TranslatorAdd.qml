import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import SFOsaka 1.0

Item {
    id: root

    signal buttonActivated()

    ColumnLayout {

        AppBar {
            id: toolbar
            onButtonActivated: root.buttonActivated()
        }

        Text {
            font.pixelSize: 12
            text: "Couldn't find what you were looking for? Please add your phrase. (Pronunciation is only necessary for Japanese)"
        }

        Row {
            id: column1

            TextField {
                id: textInput
                inputMethodHints: Qt.ImhPreferLowercase | Qt.ImhNoPredictiveText
                text: submitModel.word
                placeholderText: qsTr("New word")
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
                placeholderText: qsTr("Pronunciation")
                //font.pixelSize: 12 
                validator: SFOValidator {
                    identifier: "phonetic"
                    receiver: submitModel
                }
            }
        }
        Row {
            Text {
                text: "Add alphabetic pronunciation if needed"
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
                    placeholderText: qsTr("translation")
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
                    placeholderText: qsTr("pronunciation")
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
                    width: 15
                    text: qsTr("+")
                    id: addButton
                    onClicked: {
                        submitModel.AddTranslation(rowText.text,
                                                   rowPhonetic.text)
                    }
                }
                ToolButton {
                    visible: removeVisible
                    width: 15
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
