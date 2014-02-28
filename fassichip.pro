# check Qt version
!equals(QT_MAJOR_VERSION, 5)|lessThan(QT_MINOR_VERSION, 6) {
        error('FassiChip requires Qt version 5.2 or greater')
}

TRANSLATIONS = languages/fassichip_pt.ts

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += src/core \
    src/app \
    src/plugins
