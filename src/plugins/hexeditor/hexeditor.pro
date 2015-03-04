PRO_ROOT = $$PWD/../../..
SRC_ROOT = $$PWD/../..
QT *= core \
    gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = c4hexeditor
TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$PRO_ROOT/bin/plugins
OTHER_FILES = hexeditor.json
include(../../common.pri)
include(../hexview.pri)
SOURCES += plughexeditor.cpp \
    fhexeditor.cpp
HEADERS += plughexeditor.h \
    fhexeditor.h
RESOURCES += hexeditor.qrc
