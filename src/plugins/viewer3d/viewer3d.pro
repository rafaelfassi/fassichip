# -------------------------------------------------
# Project created by QtCreator 2011-08-04T13:00:04
# -------------------------------------------------
QT *= core \
    gui
TARGET = c4viewer3d
TEMPLATE = lib
CONFIG += plugin
include(../common.pri)
include(../chart3d.pri)
SOURCES += plugviewer3d.cpp \
    fviewer3d.cpp
HEADERS += plugviewer3d.h \
    fviewer3d.h
