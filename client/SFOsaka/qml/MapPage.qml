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

    property variant locationSF: QtPositioning.coordinate(37.78534,-122.430232)

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
            //name: "mapbox.access_token"; value: MapboxVars.Token
            name: "mapbox.access_token"; value: "pk.blahblah"
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
        // A lot of stuff changes when this is true. We need to ask permission
        // to use the location, which means NSLocationWhenInUseUsageDescription
        // and NSLocationAlwaysUsageDescription string keys need to be set.
        //active: true
        active: false
        updateInterval: 120000 // 2 mins
        onPositionChanged:  {
            //console.log("Position Changed")
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
                anchorPoint.y: contentColumn.height

                sourceItem: Column {
                    id: contentColumn
                    spacing: 4.0
                    MapPopupDescriptor {
                        id: popup
                        popupTitle: title
                        popupSubTitle: shortDescription
                        visible: isSelected
                    }
                    Image {
                        id: image
                        source: mapMarkerImage
                        MouseArea {
                            id: imageMouseArea
                            anchors.fill: parent
                            onPressed: {
                                placeModel.ToggleItemSelected(index)
                            }
                        }
                    }
                }
            }
        }

        //! [Places MapItemView]
    }
}
