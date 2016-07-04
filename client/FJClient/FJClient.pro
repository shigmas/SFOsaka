#-------------------------------------------------
#
# Project created by QtCreator 2016-06-01T15:22:25
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = FJClient
TEMPLATE = lib
CONFIG += staticlib

SOURCES += FJClient.cpp \
    FJOperation.cpp \
    FJCaller.cpp

HEADERS += FJClient.h \
    FJOperation.h \
    FJCaller.h \
    FJTypes.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
