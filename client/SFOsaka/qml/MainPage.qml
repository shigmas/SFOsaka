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
    signal transportationActivated()

    anchors.margins: 1.0

    // Use this, since Avenir is the main text on this page, so we know how big
    // we should make the description text box.
    TextMetrics {
        id: fontMetrics
        font.family: "Avenir"
        font.pointSize: 14
        text: "placeholder text"
    }

    ListModel {
        id: appSelections
        ListElement {
            iconSrc: "../resources/festival_icon.png"
            title: qsTr("Festival Schedule")
        }
        ListElement {
            iconSrc: "../resources/partners_icon.png"
            title: qsTr("Partners")
        }
        ListElement {
            iconSrc: "../resources/dictionary_icon.png"
            title: qsTr("KansaiBen Phrases")
        }
        ListElement {
            iconSrc: "../resources/scavenger_icon.png"
            title: qsTr("Scavenger Hunt")
        }
        ListElement {
            iconSrc: "../resources/partner_icon.png"
            title: qsTr("How to get there")
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignTop
        // This contains the whole screen, so fill out to take up the whole
        // space of the root item.
        anchors.fill: parent

        Image {
            id: bannerImage
            Layout.alignment: Qt.AlignTop

            // We want the image to expand to fill, but not be bigger than, the
            // width of the screen
            Layout.maximumWidth: parent.width
            // The height of this widget will expand to try and fill the space
            // that the ScrollView might default to. Meaning, we might have some
            // extra space at the top of the bottom, if we don't limit it to the
            // height of, basically, what we *should* default to, which is the
            // image size. If limit ourselves to the parent's width, and, since
            // we keep our aspect ratio, fill to the image's height. If we had
            // different size or dimensions, this probably will need to be
            // tweaked.
            Layout.maximumHeight: bannerImage.height
            width: parent.width
            source: "../resources/matsuri_banner.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: festivalDescription
            // Limit how far the width can expand. Without the maximumWidth, we
            // will exand and not wrap the lines at all
            Layout.maximumWidth: parent.width
            Layout.alignment: Qt.AlignTop
            // with our fontMetrics, we allow for 6 lines. In reality, it's more
            // like 5 linues, since there is some space between the lines.
            height: fontMetrics.height*6.0
            wrapMode: Text.Wrap
            font.pointSize: 14
            font.family: "Avenir"
            text: qsTr("Japantown Chounaikai and San Francisco Osaka Sister City Association are proud to host the third annual Osaka Matsuri on Sept 29, 2018. Come watch music and dance performances, eat Osaka food, and join us as we finish with the Bon Dance.")
        }

        ScrollView {
            id: content
            // We want to fill the remaining space, so align to the top and to
            // the bottom.
            Layout.alignment: Qt.AlignBottom | Qt.AlignTop
            Layout.fillHeight: true
            width: root.width
            // Suggested height.
            height: parent.height
            contentWidth: root.width
            // Allow the content to be more. This is arbitrary, and will be
            // insufficient if we have too many items. But, for this case, it's
            // enough.
            contentHeight: parent.height * 2;
            clip: true

            ListView {
                id: appListing
                width: parent.width
                //Layout.alignment: Qt.AlignBottom
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
                    RowLayout {
                        Image {
                            id: itemImage
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                            // Had to change this from just width and height to
                            // the max. 
                            Layout.maximumWidth: appItem.width * 0.2
                            Layout.maximumHeight: appItem.width * 0.2
                            fillMode: Image.PreserveAspectFit
                            source: iconSrc
                        }
                        Text {
                            id: itemText
                            width: appItem.width * 0.8
                            height: appItem.width * 0.2
                            //verticalAlignment: Text.AlignVCenter
                            text: title
                            style: Text.Outline
                            styleColor: "lightgrey"
                            color: "black"
                            font.family: "Avenir SE"
                            font.pointSize: 14
                        }
                        MouseArea {
                            //Layout.alignment: Qt.AlignTop | Qt.AlignBottom | Qt.AlignLeft | Qt.AlignRight
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
                                } else if (index == 4) {
                                    transportationActivated()
                                }
                            }
                        }
                    }
                }
            }
        }


        RowLayout {
            id: bottomBar
            Layout.alignment: Qt.AlignBottom

            Text {
                font.pointSize: 8
                text: "v 1.11"
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
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
                text: "2018 Futomen Networks"
            }
        }
    }
}
