TEMPLATE = app

QT += qml quick widgets network positioning location

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
}

CONFIG += c++11

SOURCES += main.cpp \
    SFOTranslateController.cpp \
    SFOItemModel.cpp \
    SFOContext.cpp \
    SFOSubmitWordModel.cpp \
    SFOEventFilter.cpp \
    SFOValidator.cpp \
    SFOValidatorReceiver.cpp \
    SFOTranslateModel.cpp \
    SFOTypes.cpp \
    SFOOrganization.cpp \
    SFOPartner.cpp \
    SFOPerformer.cpp \
    SFOScheduleModel.cpp \
    SFOController.cpp

lupdate_only{
SOURCES = FeatureRect.qml  \
    SFOSubmitWordModel.cpp \
    SFOTranslateController.cpp \
    FestivalPage.qml       \
    ListItem.qml           \
    MainPage.qml           \
    MapPage.qml            \
    MapPopupDescriptor.qml \
    PartnerDetail.qml      \
    PartnerItem.qml        \
    PartnerList.qml        \
    WebView.qml            \
    AppBar.qml             \
    TranslatorPage.qml     \
    TranslatorAdd.qml      \
    sfosaka.qml            \
}

RESOURCES += qml.qrc \
    translations.qrc \

# Additional import path used to resolve QML modules in Qt Creators code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    SFOTranslateController.h \
    SFOItemModel.h \
    SFOContext.h \
    SFOSubmitWordModel.h \
    SFOEventFilter.h \
    SFOValidator.h \
    SFOTranslateModel.h \
    SFOTypes.h \
    SFOOrganization.h \
    SFOPartner.h \
    SFOPerformer.h \
    SFOScheduleModel.h \
    SFOValidatorReceiver.h \
    SFOController.h

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
    ios/Icons/AppIcon120x120@1x.png \
    ios/LaunchScreen.xib \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \

TRANSLATIONS = translations/sfosaka_ja_JP.ts

iphonesimulator {
LIBS += -L$$PWD/../build-FJClient-iphonesimulator_clang_Qt_5_6_0_for_iOS-Debug/ -lFJClient
}
iphoneos{
LIBS += -L$$PWD/../build-FJClient-iphoneos_clang_Qt_5_6_0_for_iOS-Debug/ -lFJClient
LIBS += -L$$PWD/../build-FJClient-iphoneos_clang_Qt_5_6_0_for_iOS-Release/ -lFJClient
}
android{
#LIBS += -L$$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_6_0-Debug/ -lFJClient
LIBS += -L$$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_6_0-Release/ -lFJClient
#Debug:LIBS += -L$$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_6_0-Debug/ -lFJClient
#Release:LIBS += -L$$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_6_0-Release/ -lFJClient
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
Debug:PRE_TARGETDEPS += $$PWD/../build-FJClient-iphoneos_clang_Qt_5_6_0_for_iOS-Debug/libFJClient.a
Release:PRE_TARGETDEPS += $$PWD/../build-FJClient-iphoneos_clang_Qt_5_6_0_for_iOS-Release/libFJClient.a
}
android{
Debug:PRE_TARGETDEPS += $$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_6_0-Debug/libFJClient.a
Release:PRE_TARGETDEPS += $$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_6_0-Release/libFJClient.a
}
osx {
PRE_TARGETDEPS += $$PWD/../build-FJClient-Desktop_Qt_5_6_0_clang_64bit-Debug/libFJClient.a
}
contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        /Users/wyan/src/github/SFOsaka/client/SFOsaka/libcrypto.so \
        /Users/wyan/src/github/SFOsaka/client/SFOsaka/libssl.so
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

