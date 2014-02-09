# check Qt version
!equals(QT_MAJOR_VERSION, 4)|lessThan(QT_MINOR_VERSION, 5) {
        error('FassiChip requires Qt version 4.6 or greater')
}

TRANSLATIONS = languages/fassichip_pt.ts

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += src/app \
    src/plugins
