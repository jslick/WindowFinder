#-------------------------------------------------
#
# Project created by QtCreator 2014-10-07T20:22:51
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = windowfinder
TEMPLATE = app
CONFIG += c++11

INCLUDEPATH += ../contrib/

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    result.cpp \
    resultscollection.cpp \
    resultsfinder.cpp \
    view.cpp \
    simpleview.cpp

HEADERS += \
    mainwindow.h \
    result.h \
    resultscollection.h \
    resultsfinder.h \
    view.h \
    simpleview.h

# TODO:
LIBS += -L$$OUT_PWD/../contrib/UGlobalHotkey/debug/ -lUGlobalHotkey
