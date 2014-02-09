SRC_ROOT = ..
PRO_ROOT = ../..
QT *= core \
    gui
TARGET = fassichip
UI_DIR = $$PRO_ROOT/tmp
MOC_DIR = $$PRO_ROOT/tmp
OBJECTS_DIR = $$PRO_ROOT/tmp
DESTDIR = $$PRO_ROOT/bin
TEMPLATE = app

QMAKE_CXXFLAGS += -fPIC

INCLUDEPATH *= src \
    src/data
INCLUDEPATH *= $$SRC_ROOT \
    $$SRC_ROOT/data
DEPENDPATH *= $$SRC_ROOT \
    $$SRC_ROOT/data
SOURCES *= main.cpp \
    fmaparray.cpp \
    fdatapackage.cpp \
    fdatamap3d.cpp \
    fdataiterator.cpp \
    fdata.cpp \
    fwindowtab.cpp \
    fmainwindow.cpp \
    misc.cpp \
    finstalledpluginsdialog.cpp \
    mainwindow.cpp \
    fconfigdialog.cpp \
    QRecentFilesMenu.cpp \
    fabout.cpp
HEADERS *= version.h \
    fmaparray.h \
    fdatapackage.h \
    fdatamap3d.h \
    fdataiterator.h \
    fdata.h \
    fplugininterface.h \
    fwindowtab.h \
    fmainwindow.h \
    misc.h \
    finstalledpluginsdialog.h \
    mainwindow.h \
    fconfigdialog.h \
    QRecentFilesMenu.h \
    fabout.h
RESOURCES += app.qrc
