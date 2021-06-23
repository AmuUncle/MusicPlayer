#-------------------------------------------------
#
# Project created by QtCreator 2021-06-23T10:16:26
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
TEMPLATE = app
CONFIG += warn_off
DESTDIR = bin
unix:UI_DIR = ../tmp/ui
win32:UI_DIR = tmp/ui

SOURCES += main.cpp\
        musicplayer.cpp \
    appinit.cpp \
    playerui.cpp \
    boderpane.cpp \
    iconhelper.cpp \
    slider.cpp

HEADERS  += musicplayer.h \
    appinit.h \
    playerui.h \
    boderpane.h \
    iconhelper.h \
    slider.h

FORMS    += musicplayer.ui

RESOURCES += \
    res.qrc

RC_FILE +=  \
    musicplayer.rc
