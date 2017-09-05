import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 360
    height: 620
    id: mainWindow
    // color: "steelblue"

    StackView {
        id: stackView
        anchors.fill: parent

        signal festivalActivated()
        signal mapActivated()
        signal translatorActivated()
        signal translatorAddActivated()
        signal partnersActivated()
        signal highlightActivated()
        signal backActivated()
        signal itemSelected(int index)

        initialItem:
        MainPage {
            id: mainPage
            onFestivalActivated: stackView.festivalActivated()
            onMapActivated: stackView.mapActivated()
            onTranslatorActivated: stackView.translatorActivated()
            onPartnersActivated: stackView.partnersActivated()
            onHighlightActivated: stackView.highlightActivated()
        }

        MapPage {
            id: mapPage
            visible: false
            onButtonActivated: stackView.backActivated()
            onItemSelected: stackView.itemSelected(index)
        }

        PartnerList {
            id: partnerListPage
            visible: false
            onButtonActivated: stackView.backActivated()
        }

        PartnerDetail {
            id: partnerDetailPage
            visible: false
            onButtonActivated: stackView.backActivated()
        }

        HighlightList {
            id: highlightPage
            visible: false
            onButtonActivated: stackView.backActivated()
        }

        FestivalPage {
            id: festivalPage
            visible: false
            onButtonActivated: stackView.backActivated()
        }

        TranslatorPage {
            id: translatorPage
            visible: false
            onButtonActivated: stackView.backActivated()
            onAddActivated: stackView.translatorAddActivated()
        }

        TranslatorAdd {
            id: translatorAdd
            visible: false
            onButtonActivated: stackView.backActivated()
        }

        WebView {
            id: webView
            visible: false
            onButtonActivated: stackView.backActivated()
        }

        onFestivalActivated: {
            console.log("festival activated")
            stackView.push(festivalPage)
        }

        onMapActivated: {
            console.log("map activated")
            //stackView.push({page2: Qt.resolvedUrl("Page2.qml")})
            stackView.push(mapPage)
        }

        onItemSelected: {
            console.log(index + " chosen")
            stackView.push(partnerDetailPage)
        }
            
        onTranslatorActivated: {
            console.log("translator activated")
            stackView.push(translatorPage)
        }

        onTranslatorAddActivated: {
            console.log("translator add activated")
            stackView.push(translatorAdd)
        }

        onPartnersActivated: {
            console.log("info activated")
            stackView.push(partnerListPage)
        }

        onHighlightActivated: {
            console.log("highlight activated")
            stackView.push(highlightPage)
        }

        onBackActivated: {
            console.log("back activated")
            stackView.pop()
        }
    }
}
