PRO_ROOT = ../../..
SRC_ROOT = ../..
QT *= core \
    gui \
    xml \
    script \
    scripttools \
    uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = k4driver
TEMPLATE = lib
CONFIG += plugin

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

INCLUDEPATH += $$PRO_ROOT/qscintilla2/Qt4Qt5
DEPENDPATH += $$PRO_ROOT/qscintilla2/Qt4Qt5

win32:CONFIG(release, debug|release): LIBS += -L$$LIB_PATH/release/ -lqscintilla2
 else:win32:CONFIG(debug, debug|release): LIBS += -L$$LIB_PATH/debug/ -lqscintilla2
 else:unix: LIBS += -L$$LIB_PATH/ -lqscintilla2

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/release/qscintilla2.lib
 else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/debug/qscintilla2.lib
 else:unix:!symbian: PRE_TARGETDEPS += $$LIB_PATH/libqscintilla2.a


#unix:LIBS *= -lqscintilla2
#win32 {
#    LIBS *= $$PRO_ROOT/qscintilla/lib/libqscintilla2.a
#    DEFINES *= QT_DLL
#    INCLUDEPATH += $$PRO_ROOT/qscintilla/Qt4
#    DEPENDPATH += $$PRO_ROOT/qscintilla/Qt4
#}

OTHER_FILES += \
    driver.json
