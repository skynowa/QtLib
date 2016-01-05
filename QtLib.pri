#--------------------------------------------------------------------------------------------------
#
# \file  QtLib.pri
# \brief QtLib project file ( include($$PWD/QtLib/QtLib.pri) )
#
#--------------------------------------------------------------------------------------------------


! include(QtUtils.pri) {
    error("QtUtils.pri - not found")
}


QT += core gui widgets sql network xml

# INCLUDEPATH
INCLUDEPATH += \
    $$PWD $$PWD/../../xLib/Include

# HEADERS
HEADERS += \
    $$PWD/Common.h

qtlib_utils_on:                 HEADERS += $$PWD/Utils.h
qtlib_signal_guard_on:          HEADERS += $$PWD/SignalGuard.h
qtlib_application_on:           HEADERS += $$PWD/Application.h
qtlib_forms_about_on:           HEADERS += $$PWD/Forms/About.h
qtlib_db_sqlite_database_on:    HEADERS += $$PWD/Db/SqliteDatabase.h
qtlib_db_sql_table_model_ex_on: HEADERS += $$PWD/Db/SqlTableModelEx.h \
                                           $$PWD/Db/SqlRelationalTableModelEx.h
qtlib_db_sql_navigator_on:      HEADERS += $$PWD/Db/SqlNavigator.h

# SOURCES
qtlib_utils_on:                 SOURCES += $$PWD/Utils.cpp
qtlib_signal_guard_on:          SOURCES += $$PWD/SignalGuard.cpp
qtlib_application_on:           SOURCES += $$PWD/Application.cpp
qtlib_forms_about_on:           SOURCES += $$PWD/Forms/About.cpp
qtlib_db_sqlite_database_on:    SOURCES += $$PWD/Db/SqliteDatabase.cpp
qtlib_db_sql_table_model_ex_on: SOURCES += $$PWD/Db/SqlTableModelEx.cpp \
                                           $$PWD/Db/SqlRelationalTableModelEx.cpp
qtlib_db_sql_navigator_on:      SOURCES += $$PWD/Db/SqlNavigator.cpp

# FORMS
qtlib_forms_about_on:           FORMS += $$PWD/Forms/About.ui
