# -------------------------------------------------
# Project created by QtCreator 2011-08-04T13:00:04
# -------------------------------------------------
QT *= core \
    gui
TARGET = c4hexeditor
TEMPLATE = lib
CONFIG += plugin
include(../common.pri)
include(../hexview.pri)
SOURCES += plughexeditor.cpp \
    fhexeditor.cpp
HEADERS += plughexeditor.h \
    fhexeditor.h
RESOURCES += hexeditor.qrc
