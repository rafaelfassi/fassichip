#SRC_ROOTC = .
#PRO_ROOTC = ..
UI_DIR = $$PRO_ROOT/tmp
MOC_DIR = $$PRO_ROOT/tmp
OBJECTS_DIR = $$PRO_ROOT/tmp
#DESTDIR = $$PRO_ROOTC/bin/plugins

message($$PRO_ROOT)
message($$SRC_ROOT)

INCLUDEPATH += $$SRC_ROOT \
    $$SRC_ROOT/data
DEPENDPATH += $$SRC_ROOT \
    $$SRC_ROOT/data
SOURCES *= fmaparray.cpp \
    fdatapackage.cpp \
    fdatamap3d.cpp \
    fdataiterator.cpp \
    fdata.cpp \
    fwindowtab.cpp \
    fmainwindow.cpp \
    misc.cpp
HEADERS *= version.h \
    fmaparray.h \
    fdatapackage.h \
    fdatamap3d.h \
    fdataiterator.h \
    fdata.h \
    fplugininterface.h \
    fwindowtab.h \
    fmainwindow.h \
    misc.h
