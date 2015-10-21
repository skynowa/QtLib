#-------------------------------------------------------------------------------
#
# \file  QtLib.pri
# \brief QtLib project file ( include($$PWD/QtLib/QtLib.pri) )
#
#-------------------------------------------------------------------------------


QT += \
    core gui widgets sql network xml

HEADERS += \
    QtLib/Common.h \
    QtLib/Utils.h \
    QtLib/Application.h \
    QtLib/SqlNavigator.h \
    QtLib/Forms/About.h

SOURCES += \
    QtLib/Utils.cpp \
    QtLib/Application.cpp \
    QtLib/SqlNavigator.cpp \
    QtLib/Forms/About.cpp

FORMS += \
    QtLib/Forms/About.ui

#RESOURCES = \
#    Resources/App.qrc

#win32 {
#    RC_FILE = \
#        Resources/App_win.rc
#}

#TRANSLATIONS += \
#    $$DISTR_DIR/Langs/XXXDb_ru_RU.ts
