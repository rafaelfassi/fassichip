PRO_ROOT = ../../..
SRC_ROOT = ../..
QT *= core \
    gui
TARGET = c4hexeditor
TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$PRO_ROOT/bin/plugins
include(../../common.pri)
include(../hexview.pri)
SOURCES += plughexeditor.cpp \
    fhexeditor.cpp
HEADERS += plughexeditor.h \
    fhexeditor.h
RESOURCES += hexeditor.qrc
