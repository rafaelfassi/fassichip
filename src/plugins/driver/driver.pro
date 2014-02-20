PRO_ROOT = ../../..
SRC_ROOT = ../..
QT *= core \
    gui \
    xml \
    script \
    scripttools
TARGET = k4driver
TEMPLATE = lib
CONFIG += plugin \
    uitools
DESTDIR = $$PRO_ROOT/bin/plugins
include(../../common.pri)
SOURCES += plugdriver.cpp \
    fdriver.cpp \
    fdriverelement.cpp \
    fxmldriver.cpp \
    fdrivnewelemdialog.cpp \
    script/fscripteditor.cpp \
    script/fscriptedit.cpp \
    script/fscript.cpp
HEADERS += plugdriver.h \
    fdriver.h \
    fdriverelement.h \
    fxmldriver.h \
    fdrivnewelemdialog.h \
    script/fscripteditor.h \
    script/fscriptedit.h \
    script/fscript.h
RESOURCES += driver.qrc
unix:LIBS *= -lqscintilla2
win32 { 
    LIBS *= $$PRO_ROOT/qscintilla/lib/libqscintilla2.a
    DEFINES *= QT_DLL
    INCLUDEPATH += $$PRO_ROOT/qscintilla/Qt4
    DEPENDPATH += $$PRO_ROOT/qscintilla/Qt4
}
