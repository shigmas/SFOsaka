import QtQuick 2.4
import QtQuick.Controls 1.4
import QtWebView 1.1
import QtQuick.Layouts 1.3

// Originally had this in a layout in an item, but that doesn't work,
// so just put layout as the root (in this component).
ColumnLayout {
    id: root

    signal buttonActivated()

    AppBar {
        id: toolbar
        onButtonActivated: root.buttonActivated()
    }

    WebView {
        id: webView
        anchors.top: toolbar.bottom
        anchors.bottom: parent.bottom
        Layout.fillWidth: true
        Layout.fillHeight: true
        url: initialUrl
    }

}
