import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: root

    signal festivalActivated()
    signal mapActivated()
    signal translatorActivated()
    signal partnersActivated()
    signal highlightActivated()

    anchors.margins: 1.0
    TextMetrics {
        id: fontMetrics
        font.family: "Avenir"
        font.pointSize: 16
        text: "placeholder text"
    }

    ListModel {
        id: appSelections
        ListElement {
            iconSrc: "../resources/festival_icon.png"
            title: "Festival Schedule"
        }
        ListElement {
            iconSrc: "../resources/partners_icon.png"
            title: "Partners"
        }
        ListElement {
            iconSrc: "../resources/dictionary_icon.png"
            title: "KansaiBen Phrases"
        }
        ListElement {
            iconSrc: "../resources/taro_orange.png"
            title: "SOYNET"
        }
    }

    ColumnLayout {
        anchors.fill: parent

        ScrollView {
            //ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            width: root.width
            Layout.fillHeight: true
            Layout.fillWidth: true
            // The *visible* height
            contentHeight: parent.height
            // The canvas height
            height: parent.height*2
            clip: true

            Item {
                id: scrollableItem
                width: root.width

                Image {
                    id: bannerImage
                    anchors.top: parent.top
                    width: root.width
                    //height: parent.width*.483
                    source: "../resources/matsuri_banner.png"
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    id: festivalDescription
                    anchors.top: bannerImage.bottom
                    width: root.width
                    height: fontMetrics.height*6.0
                    wrapMode: Text.Wrap
                    font.pointSize: 14
                    font.family: "Avenir"
                    text: qsTr("This year's Osaka Matsuri will be held Sept 23, 2017. Osaka and San Francisco have enjoyed a 60 year relationship as sister cities. We will be celebrating by highlighting Osaka and Japanese culture.")
                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            console.log("(" + parent.width + ", " + parent.height + ")")
                        }
                    }
                }

                ListView {
                    id: appListing
                    anchors.top: festivalDescription.bottom
                    width: parent.width
                    height: parent.width
                    model: appSelections
                    // Super simple: this listing is short and sweet
                    delegate: Item {
                        id: appItem
                        // We need the width and especially height. I think it's
                        // because we're in a scroll view, but we also need a
                        // height limiter because, even though we set the height
                        // in the child, it will still try to expand if the
                        // parent doesn't rein it in.
                        width: parent.width
                        height: parent.width * 0.2
                        implicitHeight: contentItem.height
                        Image {
                            id: itemImage
                            anchors.left: parent.left
                            width: appItem.width * 0.2
                            height: appItem.width * 0.2
                            fillMode: Image.PreserveAspectFit
                            source: iconSrc
                        }
                        Text {
                            id: itemText
                            anchors.left: itemImage.right
                            anchors.right: parent.right
                            width: appItem.width * 0.8
                            height: appItem.width * 0.2
                            verticalAlignment: Text.AlignVCenter
                            text: title
                            style: Text.Outline
                            styleColor: "lightgrey"
                            color: "black"
                            font.family: "Avenir SE"
                            font.pointSize: 14
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("Index: " + index)
                                if (index == 0) {
                                    festivalActivated()
                                } else if (index == 1) {
                                    partnersActivated()
                                } else if (index == 2) {
                                    translatorActivated()
                                } else if (index == 3) {
                                    highlightActivated()
                                }
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Text {
                font.pointSize: 8
                text: "v 1.7"
                anchors.left: parent.left
                MouseArea {
                    id: versionMouseArea
                    anchors.fill: parent
                    onPressed: {
                        globalController.HandleRefresh()
                    }
                }
            }

            Rectangle {
                width: 40
                height: 0
            }

            Text {
                font.pointSize: 8
                text: server
            }
            Rectangle {
                width: 15
                height: 0
            }
            Text {
                font.pointSize: 8
                text: "2016 Futomen Networks"
            }
        }
    }
}
