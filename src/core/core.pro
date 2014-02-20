SRC_ROOT = ..
PRO_ROOT = ../..

QT *= core \
    gui \

TARGET = core
TEMPLATE = lib

CONFIG += staticlib

MOC_DIR = $$PRO_ROOT/tmp
OBJECTS_DIR = $$PRO_ROOT/tmp
DESTDIR = $$PRO_ROOT/bin/lib

INCLUDEPATH += ./data
DEPENDPATH += ./data

SOURCES *= fmaparray.cpp \
    fdatapackage.cpp \
    fdatamap3d.cpp \
    fdataiterator.cpp \
    fdata.cpp \
    fwindowtab.cpp \
    fmainwindow.cpp \
    misc.cpp

HEADERS *= version.h \
    fmaparray.h \
    fdatapackage.h \
    fdatamap3d.h \
    fdataiterator.h \
    fdata.h \
    fplugininterface.h \
    fwindowtab.h \
    fmainwindow.h \
    misc.h
