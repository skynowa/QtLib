#---------------------------------------------------------------------------
# \file  iTeacher.pro
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
    SOURCES += \
        $$PWD/GlobalShortcut_win.cpp
    LIBS += \
        -luser32
}

unix {
    SOURCES += \
        $$PWD/GlobalShortcut_x11.cpp
}

mac {
    SOURCES += \
        $$PWD/GlobalShortcut_mac.cpp
}
