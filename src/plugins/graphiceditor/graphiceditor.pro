# -------------------------------------------------
# Project created by QtCreator 2011-08-04T13:00:04
# -------------------------------------------------
QT *= core \
    gui
TARGET = c4graphiceditor
TEMPLATE = lib
CONFIG += plugin
include(../common.pri)
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
