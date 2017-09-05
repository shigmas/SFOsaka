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
    src/SFOPerformer.h \
    src/SFOScheduleModel.h \
    src/SFOValidatorReceiver.h \
    src/SFOController.h

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

iphonesimulator {
    CONFIG(debug, debug|release) {
        LIBS += -L$$PWD/../build-FJClient-iphonesimulator_clang_Qt_5_9_0_for_iOS-Debug/ -lFJClient
        PRE_TARGETDEPS += $$PWD/../build-FJClient-iphonesimulator_clang_Qt_5_9_0_for_iOS-Debug/libFJClient.a
    } else {
        LIBS += -L$$PWD/../build-FJClient-iphonesimulator_clang_Qt_5_9_0_for_iOS-Debug/ -lFJClient
        PRE_TARGETDEPS += $$PWD/../build-FJClient-iphonesimulator_clang_Qt_5_9_0_for_iOS-Debug/libFJClient.a
    }
}

iphoneos {
    CONFIG(debug, debug|release) {
        PRE_TARGETDEPS += $$PWD/../build-FJClient-iphoneos_clang_Qt_5_9_0_for_iOS-Debug/libFJClient.a
        LIBS += -L$$PWD/../build-FJClient-iphoneos_clang_Qt_5_9_0_for_iOS-Debug/ -lFJClient
    } else { # CONFIG(release, debug|release)
        PRE_TARGETDEPS += $$PWD/../build-FJClient-iphoneos_clang_Qt_5_9_0_for_iOS-Release/libFJClient.a
        LIBS += -L$$PWD/../build-FJClient-iphoneos_clang_Qt_5_9_0_for_iOS-Release/ -lFJClient
    }
}

android {
    CONFIG(debug, debug|release) {
        message(DEBUG)
        PRE_TARGETDEPS += $$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_9_0_for_Android_armv7-Debug/libFJClient.a
        LIBS += -L$$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_9_0_for_Android_armv7-Debug/ -lFJClient
    } else {
        message(RELEASE)
        PRE_TARGETDEPS += $$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_9_0_for_Android_armv7-Release/libFJClient.a
        LIBS += -L$$PWD/../build-FJClient-Android_for_armeabi_v7a_GCC_4_9_Qt_5_9_0_for_Android_armv7-Release/ -lFJClient
    }
}        

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        /Users/wyan/src/github/SFOsaka/client/SFOsaka/libcrypto.so \
        /Users/wyan/src/github/SFOsaka/client/SFOsaka/libssl.so
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

