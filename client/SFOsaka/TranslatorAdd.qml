import QtQuick 2.4
import QtQuick.Controls 1.4

Item {
    id: root

    Column {
        id: row1

        Row {
            id: column1

            TextField {
                id: textInput
                text: qsTr("Text Edit")
                //font.pixelSize: 12
                Keys.onPressed: {
                    submitModel.word = text
                }

                onEditingFinished: {
                    submitModel.word = text
                }
            }

            ToolButton {
                id: toolButton1
            }
            Button {
                id: submitButton
                text: qsTr("Submit")
             }
            Button {
                id: cancelButton
                text: qsTr("Cancel")
                onClicked: {
                    stackView.pop()
                }

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
                    text: translation
                    width: 120
                    height: 40
                }
                Button {
                    visible: addVisible
                    text: qsTr("Add")
                    id: toolButton1
                    onClicked: {
                        submitModel.AddTranslation(rowText.text)
                    }
                }
            }
            model: submitModel
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
