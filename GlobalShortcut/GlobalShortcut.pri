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
    $$PWD/GlobalShortcut.h
    $$PWD/x11/QxtX11ErrorHandler.h
    $$PWD/x11/QxtX11Data.h

SOURCES += \
    $$PWD/GlobalShortcut.cpp
    $$PWD/x11/QxtX11ErrorHandler.cpp
    $$PWD/x11/QxtX11Data.cpp

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
