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
    $$PWD/x11/X11ErrorHandler.h \
    $$PWD/x11/X11Data.h \
    $$PWD/x11/ShortcutActivator.h

SOURCES += \
    $$PWD/GlobalShortcut.cpp \
    $$PWD/x11/X11ErrorHandler.cpp \
    $$PWD/x11/X11Data.cpp \
    $$PWD/x11/ShortcutActivator.cpp

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
