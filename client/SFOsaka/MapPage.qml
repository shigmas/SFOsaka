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
    signal itemSelected(int index)

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

        property var selectedItem
        property real popupYOffset

        gesture.acceptedGestures: MapGestureArea.PanGesture | MapGestureArea.FlickGesture | MapGestureArea.PinchGesture
        gesture.enabled: true

        gesture.onPanFinished: {
            if (popup.visible) {
                popup.xPos = selectedItem.x
                popup.yPos = selectedItem.y + popupYOffset
            }
        }
        gesture.onPinchFinished: {
            if (popup.visible) {
                popup.xPos = selectedItem.x
                popup.yPos = selectedItem.y + popupYOffset
            }
        }
        gesture.onFlickFinished: {
            if (popup.visible) {
                popup.xPos = selectedItem.x
                popup.yPos = selectedItem.y + popupYOffset
            }
        }

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
                            map.selectedItem = mapItem
                            map.popupYOffset = image.height * 0.5
                            popup.xPos = mapItem.x
                            popup.yPos = mapItem.y + image.height * 0.5
                            popup.title = title
                            popup.shortDescription = shortDescription
                            popup.visible = true
                        }
                    }

                }
            }
        }

        MouseArea {
            id: mapMouseArea
            anchors.fill: parent
            onPressed: {
                // Unselection doesn't work - box gets drawn in all sorts of
                // weird ways.
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
