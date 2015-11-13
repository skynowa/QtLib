#-------------------------------------------------------------------------------
#
# \file  QtLib.pri
# \brief QtLib project file ( include($$PWD/QtLib/QtLib.pri) )
#
#-------------------------------------------------------------------------------


! include(QtUtils.pri) {
    error("QtUtils.pri - not found")
}


QT += core gui widgets sql network xml

# INCLUDEPATH
INCLUDEPATH += \
    $$PWD ../../xLib/Include

# HEADERS
HEADERS += \
    $$PWD/Common.h

qtlib_utils_on:         HEADERS += $$PWD/Utils.h
qtlib_signal_guard_on:  HEADERS += $$PWD/SignalGuard.h
qtlib_application_on:   HEADERS += $$PWD/Application.h
qtlib_sql_navigator_on: HEADERS += $$PWD/SqlNavigator.h
qtlib_forms_about_on:   HEADERS += $$PWD/Forms/About.h

# SOURCES
qtlib_utils_on:         SOURCES += $$PWD/Utils.cpp
qtlib_signal_guard_on:  SOURCES += $$PWD/SignalGuard.cpp
qtlib_application_on:   SOURCES += $$PWD/Application.cpp
qtlib_sql_navigator_on: SOURCES += $$PWD/SqlNavigator.cpp
qtlib_forms_about_on:   SOURCES += $$PWD/Forms/About.cpp

# FORMS
qtlib_forms_about_on:   FORMS += $$PWD/Forms/About.ui
