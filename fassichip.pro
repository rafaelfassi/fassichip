# check Qt version
!equals(QT_MAJOR_VERSION, 5)|lessThan(QT_MINOR_VERSION, 1) {
        error('FassiChip requires Qt version 5.1 or greater')
}

TRANSLATIONS = languages/fassichip_pt.ts

TEMPLATE  = subdirs
CONFIG   += ordered

SUBDIRS += src/core \
    src/app \
    src/plugins


documentation.path = $$OUT_PWD/bin
documentation.files = $$PWD/doc
languages.path = $$OUT_PWD/bin
languages.files = $$PWD/languages
examples.path = $$OUT_PWD/bin
examples.files = $$PWD/examples

INSTALLS += documentation languages examples
