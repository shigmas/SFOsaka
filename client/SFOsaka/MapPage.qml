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
    //anchors.fill: parent

    signal buttonActivated()
    signal itemSelected(string title)

    AppBar {
        id: toolbar
        anchors.top: parent.top
        onButtonActivated: root.buttonActivated()
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

    // add 0.01 to latitude to move east a little bit
    property variant locationSF: QtPositioning.coordinate( 37.785614,-122.42501)
//    property variant locationSF: QtPositioning.coordinate( 59.93, 10.75)

    //! [Places MapItemView]
    Map {
        id: map

        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        width: parent.width
        plugin: myPlugin;
        center: locationSF
        zoomLevel: 15

        MapItemView {
            model: placeModel
            delegate: MapQuickItem {
                coordinate: coord

                anchorPoint.x: image.width * 0.5
                anchorPoint.y: image.height

                sourceItem: Column {
                    Image {
                        id: image
                        source: "resources/marker.png"
                        MouseArea {
                            id: imageMouseArea
                            anchors.fill: parent
                            //onPressed: itemSelected(title)
                        }
                    }
                    Text {
                        text: title
                        font.bold: true
                        MouseArea {
                            id: textMouseArea
                            anchors.fill: parent
                            //onPressed: itemSelected(title)
                        }
                        Connections {
                            target: imageMouseArea
                            onPressed: {
                                console.log(title + " selected");
                                placeModel.HandleItemSelected(title)
                                itemSelected(title)
                            }
                        }
                        
                    }
                }
            }
        }
        //! [Places MapItemView]
    }

}
