#---------------------------------------------------------------------------
# \file  GlobalShortcut.pro
# \brief project file
#---------------------------------------------------------------------------


QT += \
    gui-private

INCLUDEPATH += \
    $$PWD

DEPENDPATH += \
    $$PWD

HEADERS += \
    $$PWD/Global.h \
    $$PWD/GlobalShortcut.h \
    $$PWD/GlobalShortcut_impl.h

SOURCES += \
    $$PWD/GlobalShortcut.cpp

win32 {
    LIBS += \
        -luser32
}

unix {
    LIBS += \
        -lX11
}

mac {
}
