import QtQuick 2.4
import QtQuick.Controls 1.4
import QtPositioning 5.5
import QtLocation 5.6
//! [Imports]

Item {

    id: root
    signal buttonActivated()
    signal itemSelected(string title)

    //! [Initialize Plugin]
    Plugin {
        id: myPlugin
        name: "osm"
        //specify plugin parameters if necessary
        //PluginParameter {...}
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

        anchors.fill: parent
        plugin: myPlugin;
        center: locationSF
        zoomLevel: 13

        MapItemView {
            model: placeModel
            delegate: MapQuickItem {
                coordinate: coord

                anchorPoint.x: image.width * 0.5
                anchorPoint.y: image.height

                sourceItem: Column {
                    Image {
                        id: image
                        source: "marker.png"
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
            /*
            delegate: MapCircle {
                center: coord
                radius: 55.0
            }
            */
        }
        //! [Places MapItemView]
    }

    Row {

    Button {
        id: button1
        text: qsTr("go back")
        onClicked: root.buttonActivated()
    }

    Button {
        id: button2
        text: qsTr("Fetch")
        onClicked: placeModel.HandleRefresh()
    }

}
    /*
    Connections {
        target: placeModel
        onPositionChanged: {
            console.log("positionChanged");
            map.update()
        }
    }
    */
}
