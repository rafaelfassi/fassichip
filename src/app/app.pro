SRC_ROOT = $$PWD/..
PRO_ROOT = $$PWD/../..
QT *= core \
    gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fassichip
UI_DIR = $$PRO_ROOT/tmp
MOC_DIR = $$PRO_ROOT/tmp
OBJECTS_DIR = $$PRO_ROOT/tmp
DESTDIR = $$PRO_ROOT/bin
TEMPLATE = app

#QMAKE_CXXFLAGS += -fPIC

INCLUDEPATH *= $$SRC_ROOT \
    $$SRC_ROOT/data
DEPENDPATH *= $$SRC_ROOT \
    $$SRC_ROOT/data

include(../common.pri)

SOURCES *= main.cpp \
    finstalledpluginsdialog.cpp \
    mainwindow.cpp \
    fconfigdialog.cpp \
    QRecentFilesMenu.cpp \
    fabout.cpp
HEADERS *= finstalledpluginsdialog.h \
    mainwindow.h \
    fconfigdialog.h \
    QRecentFilesMenu.h \
    fabout.h
RESOURCES += app.qrc
