LIB_PATH = $$PRO_ROOT/bin/lib
UI_DIR = $$PRO_ROOT/tmp
MOC_DIR = $$PRO_ROOT/tmp
OBJECTS_DIR = $$PRO_ROOT/tmp

#message($$PRO_ROOT)
#message($$SRC_ROOT)

INCLUDEPATH += $$SRC_ROOT \
    $$SRC_ROOT/core \
    $$SRC_ROOT/core/data

DEPENDPATH += $$SRC_ROOT \
    $$SRC_ROOT/core \
    $$SRC_ROOT/core/data

HEADERS *= version.h

win32:CONFIG(release, debug|release): LIBS += -L$$LIB_PATH/release/ -lcore
 else:win32:CONFIG(debug, debug|release): LIBS += -L$$LIB_PATH/debug/ -lcore
 else:unix: LIBS += -L$$LIB_PATH/ -lcore

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/release/core.lib
 else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/debugcore.lib
 else:unix:!symbian: PRE_TARGETDEPS += $$LIB_PATH/libcore.a
