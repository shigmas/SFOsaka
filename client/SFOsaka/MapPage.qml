import QtQuick 2.4
import QtQuick.Controls 1.4
import QtPositioning 5.5
import QtLocation 5.6
import QtQuick.Layouts 1.3

//! [Imports]

// QTBUG-34418: singletons need to be imported
import "."

ColumnLayout {

    id: root
    Layout.fillWidth: true
    anchors.fill: parent

    property variant locationSF: QtPositioning.coordinate( 37.785124,-122.431322)

    signal buttonActivated()
    signal itemSelected(string title)

    AppBar {
        id: toolbar
        anchors.top: parent.top
        onButtonActivated: {
            placeModel.HandleItemSelected(-1)
            root.buttonActivated()
        }
    }

    //! [Initialize Plugin]
    Plugin {
        id: myPlugin
        //name: "osm"
        name: "mapbox"
        //specify plugin parameters if necessary
        PluginParameter {
            // Tried importing the variable as javascript, and as a QML singleton
            // with no success. (The variable exists, but it's not a useful
            // string. So, you just have to enter this value by hand
            name: "mapbox.access_token"; value: MapboxVars.Token
        }
        PluginParameter {
            name: "mapbox.map_id"; value: "mapbox.streets"
        }
        //PluginParameter {...}
        //...
    }
    //! [Initialize Plugin]

    //! [Current Location]
    PositionSource {
        id: positionSource
        property variant lastSearchPosition: locationSF
        active: true
        updateInterval: 120000 // 2 mins
        onPositionChanged:  {
            console.log("Position Changed")
            var currentPosition = positionSource.position.coordinate
            map.center = currentPosition
            var distance = currentPosition.distanceTo(lastSearchPosition)
            if (distance > 500) {
                // 500m from last performed pizza search
                lastSearchPosition = currentPosition
            }
            // force an update of drawing the model items.
            placeModel.position = map.center
        }
    }
    //! [Current Location]

    //! [Places MapItemView]
    Map {
        id: map

        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        plugin: myPlugin
        center: locationSF
        zoomLevel: 17

        MapItemView {
            model: placeModel
            delegate: MapQuickItem {
                id: mapItem
                coordinate: coord
                anchorPoint.x: image.width * 0.5
                anchorPoint.y: image.height

                sourceItem: Column {
                    Rectangle {
                        property string title
                        id: descriptionRect
                        border.width: 4
                        border.color: "black"
                        visible: false
                        y: -(image.height)
                        Text {
                            text: title
                        }
                    }

                    Image {
                        id: image
                        source: mapMarkerImage
                        MouseArea {
                            id: imageMouseArea
                            anchors.fill: parent
                            onPressed: {
                            }
                        }
                    }

                    Connections {
                        target: imageMouseArea
                        onPressed: {
                            placeModel.HandleItemSelected(index)
                            console.log("Popup: (" + mapItem.x + ", " + mapItem.y + image.height * 0.5 + ")")
                            console.log("selected " + index)
                            console.log("title " + title)
                            console.log("desc " + shortDescription)
                            console.log("offset: " + image.height * 0.5)
                            popup.xPos = mapItem.x
                            popup.yPos = mapItem.y + image.height * 0.5
                            popup.title = title
                            popup.shortDescription = shortDescription
                            popup.visible = true
                            console.log("title height: " + popup.titleFontHeight)
                            console.log("title width: " + popup.titleFontWidth)
                            console.log("w/h: (" + popup.width + ", " + popup.height + ")")
                        }
                    }

                }
            }
        }
        MouseArea {
            id: mapMouseArea
            anchors.fill: parent
            onPressed: {
                console.log("nothing pressed")
                console.log("Current pop location: (" + popup.x + ", " + popup.y + ")")
                console.log("title height: " + popup.titleFontHeight)
                console.log("title width: " + popup.titleFontWidth)
                console.log("w/h: (" + popup.width + ", " + popup.height + ")")
                //placeModel.HandleItemSelected(-1)
                //popup.visible = false
            }

        }
        //! [Places MapItemView]
    }
    MapPopupDescriptor {
        id: popup
    }
}
