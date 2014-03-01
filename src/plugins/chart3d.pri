
INCLUDEPATH += $$SRC_ROOT/chart3d \
    $$PRO_ROOT/qwtplot3d/include

DEPENDPATH += $$SRC_ROOT/chart3d \
    $$PRO_ROOT/qwtplot3d/include

SOURCES *= $$SRC_ROOT/chart3d/fchart3d.cpp

HEADERS *= $$SRC_ROOT/chart3d/fchart3d.h

RESOURCES *= $$SRC_ROOT/chart3d/chart3d.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$LIB_PATH/release/ -lqwtplot3d
 else:win32:CONFIG(debug, debug|release): LIBS += -L$$LIB_PATH/debug/ -lqwtplot3d
 else:unix: LIBS += -L$$LIB_PATH/ -lqwtplot3d

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$LIB_PATH/release/qwtplot3d.lib
 else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$LIB_PATH/debug/qwtplot3d.lib
 else:unix:!symbian: PRE_TARGETDEPS += $$LIB_PATH/libqwtplot3d.a

QT *= opengl

CONFIG *= opengl

unix:LIBS += -lGLU
