TEMPLATE = app

QT += qml quick network positioning location

!osx:qtHaveModule(webengine) {
        QT += webengine
        DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
}

ios {
    QMAKE_INFO_PLIST = ios/Info.plist

    ios_icon.files = $$files($$PWD/ios/Icons/AppIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
}

CONFIG += c++11

SOURCES += main.cpp \
    SFOTranslateController.cpp \
    SFOItemModel.cpp \
    SFOContext.cpp \
    SFOPartner.cpp \
    SFOSubmitWordModel.cpp

lupdate_only{
SOURCES = MainPage.qml \
    MapPage.qml        \
    SisterPage.qml     \
    TranslatorPage.qml \
    TranslatorAdd.qml  \
    sfosaka.qml        \
}

RESOURCES += qml.qrc \
    translations.qrc \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    SFOTranslateController.h \
    SFOItemModel.h \
    SFOContext.h \
    SFOPartner.h \
    SFOSubmitWordModel.h

DISTFILES += \
    ios/Icons/AppIcon60x60@3x.png \
    ios/Icons/AppIcon40x40@3x.png \
    ios/Icons/AppIcon40x40@2x.png \
    ios/Icons/AppIcon29x29@2x.png \
    ios/Icons/AppIcon76x76@1x.png \
    ios/Icons/AppIcon76x76@2x.png \
    ios/Icons/AppIcon57x57@1x.png \
    ios/Icons/AppIcon72x72@1x.png \
    ios/Icons/AppIcon83.5x83.5@2x.png \

TRANSLATIONS = translations/oscity_ja_JP.ts

iphonesimulator {
LIBS += -L$$PWD/../build-FJClient-iphonesimulator_clang_Qt_5_6_0_for_iOS-Debug/ -lFJClient
}
iphoneos{
LIBS += -L$$PWD/../build-FJClient-iphoneos_clang_Qt_5_6_0_for_iOS-Debug/ -lFJClient
}
android{
LIBS += -L$$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_6_0-Debug/ -lFJClient
}
osx{
LIBS += -L$$PWD/../build-FJClient-Desktop_Qt_5_6_0_clang_64bit-Debug/ -lFJClient
}


INCLUDEPATH += $$PWD/../FJClient
DEPENDPATH += $$PWD/../FJClient

iphonesimulator {
PRE_TARGETDEPS += $$PWD/../build-FJClient-iphonesimulator_clang_Qt_5_6_0_for_iOS-Debug/libFJClient.a
}
iphoneos{
PRE_TARGETDEPS += $$PWD/../build-FJClient-iphoneos_clang_Qt_5_6_0_for_iOS-Debug/libFJClient.a
}
android{
PRE_TARGETDEPS += $$PWD/../build-FJClient-Desktop_Qt_5_6_0_clang_64bit-Debug/libFJClient.a
}

