PRO_ROOT = ../../..
SRC_ROOT = ../..
QT *= core \
    gui
TARGET = c4viewer3d
TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$PRO_ROOT/bin/plugins
include(../../common.pri)
include(../chart3d.pri)
SOURCES += plugviewer3d.cpp \
    fviewer3d.cpp
HEADERS += plugviewer3d.h \
    fviewer3d.h
