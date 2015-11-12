#-------------------------------------------------------------------------------
#
# \file  QtLib.pri
# \brief QtLib project file ( include($$PWD/QtLib/QtLib.pri) )
#
#-------------------------------------------------------------------------------


! include(QtUtils.pri) {
    error("QtUtils.pri - not found")
}


QT += \
    core gui widgets sql network xml

INCLUDEPATH += \
    $$PWD ../../xLib/Include

HEADERS += \
    $$PWD/Common.h \
    $$PWD/Utils.h \
    $$PWD/SignalGuard.h \
    $$PWD/Application.h \
    $$PWD/SqlNavigator.h \
    $$PWD/Forms/About.h

SOURCES += \
    $$PWD/Utils.cpp \
    $$PWD/SignalGuard.cpp \
    $$PWD/Application.cpp \
    $$PWD/SqlNavigator.cpp \
    $$PWD/Forms/About.cpp

FORMS += \
    $$PWD/Forms/About.ui
