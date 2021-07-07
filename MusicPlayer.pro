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

INCLUDEPATH += include

HEADERS += include/musicplayer.h \
            include/appinit.h \
            include/playerui.h \
            include/boderpane.h \
            include/iconhelper.h \
            include/slider.h \
            include/libzplay.h \
            include/musicmgr.h \
            include/cfgdlg.h

SOURCES += source/main.cpp\
        source/musicplayer.cpp \
        source/appinit.cpp \
        source/playerui.cpp \
        source/boderpane.cpp \
        source/iconhelper.cpp \
        source/slider.cpp \
        source/musicmgr.cpp \
        source/cfgdlg.cpp

FORMS    += ui/musicplayer.ui \
    ui/cfgdlg.ui

RESOURCES += \
    res/res.qrc

RC_FILE +=  \
    res/musicplayer.rc

win32: LIBS += -L$$PWD/lib/ -llibzplay

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
