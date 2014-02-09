SRC_ROOT = ../..
PRO_ROOT = ../../..

INCLUDEPATH += $$SRC_ROOT/chart3d

DEPENDPATH += $$SRC_ROOT/chart3d

SOURCES *= fchart3d.cpp

HEADERS *= fchart3d.h

RESOURCES *= chart3d.qrc

unix {
    LIBS *= -lqwtplot3d
}
win32 {
    LIBS *= $$PRO_ROOT/qwtplot3d/lib/libqwtplot3d.a
    DEFINES *= QT_DLL \
        QWT3D_DLL
    INCLUDEPATH += $$PRO_ROOT/qwtplot3d/include
    DEPENDPATH += $$PRO_ROOT/qwtplot3d/include
}

QT *= opengl

CONFIG *= opengl
