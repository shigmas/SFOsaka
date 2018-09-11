TEMPLATE = app

QT += qml quick widgets network positioning location webview

!osx:qtHaveModule(webengine) {
        QT += webengine
        DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
}

ios {
    QMAKE_INFO_PLIST = ios/Info.plist

    ios_icon.files = $$files($$PWD/ios/Icons/AppIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon

    launch_images.files = $$files($PWD/ios/LaunchScreen.xib) $$files($$PWD/resources/bmatsuri_banner.png)
    QMAKE_BUNDLE_DATA += launch_images

	QMAKE_ASSET_CATALOGS = $$PWD/ios/Images.xcassets
	QMAKE_ASSET_CATALOGS_APP_ICON = "AppIcon"
}

CONFIG += c++11

SOURCES += src/main.cpp \
    src/SFOTranslateController.cpp \
    src/SFOItemModel.cpp \
    src/SFOIgnoreNetworkAccessManager.cpp \
    src/SFOIgnoreNetworkFactory.cpp \
    src/SFOContext.cpp \
    src/SFOSubmitWordModel.cpp \
    src/SFOEventFilter.cpp \
    src/SFOValidator.cpp \
    src/SFOValidatorReceiver.cpp \
    src/SFOTranslateModel.cpp \
    src/SFOTypes.cpp \
    src/SFOOrganization.cpp \
    src/SFOAppHighlight.cpp \
    src/SFOTransportation.cpp \
    src/SFOPartner.cpp \
    src/SFOPerformer.cpp \
    src/SFOScheduleModel.cpp \
    src/SFOController.cpp

lupdate_only{
SOURCES = qml/FeatureRect.qml  \
    src/SFOSubmitWordModel.cpp \
    src/SFOTranslateController.cpp \
    qml/FestivalPage.qml       \
    qml/ListItem.qml           \
    qml/MainPage.qml           \
    qml/MapPage.qml            \
    qml/MapPopupDescriptor.qml \
    qml/PartnerDetail.qml      \
    qml/PartnerItem.qml        \
    qml/PartnerList.qml        \
    qml/HighlightList.qml      \
    qml/GenericItem.qml        \
    qml/TransportationList.qml \
    qml/WebView.qml            \
    qml/AppBar.qml             \
    qml/TranslatorPage.qml     \
    qml/TranslatorAdd.qml      \
    qml/sfosaka.qml            \
}

RESOURCES += qml.qrc \
    translations.qrc \

# Additional import path used to resolve QML modules in Qt Creators code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/SFOTranslateController.h \
    src/SFOItemModel.h \
    src/SFOIgnoreNetworkAccessManager.h \
    src/SFOIgnoreNetworkFactory.h \
    src/SFOContext.h \
    src/SFOSubmitWordModel.h \
    src/SFOEventFilter.h \
    src/SFOValidator.h \
    src/SFOTranslateModel.h \
    src/SFOTypes.h \
    src/SFOOrganization.h \
    src/SFOAppHighlight.h \
    src/SFOPartner.h \
    src/SFOTransportation.h \
    src/SFOPerformer.h \
    src/SFOScheduleModel.h \
    src/SFOValidatorReceiver.h \
    src/SFOController.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \

TRANSLATIONS = translations/sfosaka_ja_JP.ts


INCLUDEPATH += $$PWD/../FJClient
DEPENDPATH += $$PWD/../FJClient

# New build uses standardized directories, so just expect that it's in the
# next directory from the out dir.
LIBS += -L$$OUT_PWD/../FJClient -lFJClient
#PRE_TARGETDEPS += $$OUT_PWD/../FJClient/libFJClient.a

# Move these here by hand
contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$OUT_PWD/../libs/libcrypto.so \
        $$OUT_PWD/../libs/libssl.so
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

