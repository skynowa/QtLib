#-------------------------------------------------------------------------------
#
# \file  QtLib.pri
# \brief QtLib project file ( include($$PWD/QtLib/QtLib.pri) )
#
#-------------------------------------------------------------------------------


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

#RESOURCES = \
#    $$PWD/Resources/App.qrc

#win32 {
#    RC_FILE = \
#        $$PWD/Resources/App_win.rc
#}

#TRANSLATIONS += \
#    $$DISTR_DIR/Langs/XXXDb_ru_RU.ts