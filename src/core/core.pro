SRC_ROOT = $$PWD/..
PRO_ROOT = $$PWD/../..

QT += core \
    gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = core
TEMPLATE = lib

CONFIG += staticlib

MOC_DIR = $$PRO_ROOT/tmp
OBJECTS_DIR = $$PRO_ROOT/tmp

win32:CONFIG(release, debug|release): DESTDIR += $$PRO_ROOT/bin/lib/release
 else:win32:CONFIG(debug, debug|release): DESTDIR += $$PRO_ROOT/bin/lib/debug
 else:unix:!symbian: DESTDIR = $$PRO_ROOT/bin/lib

INCLUDEPATH += data
DEPENDPATH += data

SOURCES *= data/fmaparray.cpp \
    data/fdatapackage.cpp \
    data/fdatamap3d.cpp \
    data/fdataiterator.cpp \
    data/fdata.cpp \
    fwindowtab.cpp \
    fmainwindow.cpp \
    misc.cpp

HEADERS *= data/fmaparray.h \
    data/fdatapackage.h \
    data/fdatamap3d.h \
    data/fdataiterator.h \
    data/fdata.h \
    fplugininterface.h \
    fwindowtab.h \
    fmainwindow.h \
    misc.h
