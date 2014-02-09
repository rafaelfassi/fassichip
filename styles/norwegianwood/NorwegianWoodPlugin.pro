QT += core \
    gui
TARGET = NorwegianWoodPlugin
TEMPLATE = lib
CONFIG += plugin
HEADERS += norwegianwoodstyle.h \
    norwegianwoodplugin.h
SOURCES += norwegianwoodstyle.cpp \
    norwegianwoodplugin.cpp
DESTDIR = ../../bin/styles
RESOURCES += NorwegianWoodPlugin.qrc
