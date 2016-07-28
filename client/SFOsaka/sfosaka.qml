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
        signal infoActivated()
        signal backActivated()
        signal itemSelected(string title)

        initialItem:
        MainPage {
            id: mainPage
            onFestivalActivated: stackView.festivalActivated()
            onMapActivated: stackView.mapActivated()
            onTranslatorActivated: stackView.translatorActivated()
            onInfoActivated: stackView.infoActivated()
        }

        MapPage {
            id: mapPage
            visible: false
            onButtonActivated: stackView.backActivated()
            onItemSelected: stackView.itemSelected(title)
        }

        PartnerDetail {
            id: partnerDetailPage
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

        SisterPage {
            id: infoPage
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
            console.log(title + " chosen")
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

        onInfoActivated: {
            console.log("info activated")
            stackView.push(infoPage)
        }

        onBackActivated: {
            console.log("back activated")
            stackView.pop()
        }
    }
}
