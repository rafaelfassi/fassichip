PRO_ROOT = ../../..
SRC_ROOT = ../..
QT *= core \
    gui
TARGET = c4graphiceditor
TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$PRO_ROOT/bin/plugins
include(../../common.pri)
include(../chart2d.pri)
include(../chart3d.pri)
SOURCES += pluggraphiceditor.cpp \
    fgraphicedttool.cpp \
    fgraphiceditor.cpp \
    fgraphiceditorcfg.cpp
HEADERS += pluggraphiceditor.h \
    fgraphicedttool.h \
    fgraphiceditor.h \
    fgraphiceditorcfg.h
RESOURCES += graphiceditor.qrc
