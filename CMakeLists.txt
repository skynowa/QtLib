#--------------------------------------------------------------------------------------------------
# \file  CMakeLists.txt
# \brief QtLib
#
# PARENT_SCOPE vars:
#   ${QTLIB_MOC_UI}
#   ${QTLIB_MOC_CPP}
#   ${QTLIB_CPP}
#   ${QTLIB_UI_CPP}
#
#--------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_LIST_DIR})

set(QTLIB_MOC_UI
    ${CMAKE_CURRENT_LIST_DIR}/Ui/About.ui
    PARENT_SCOPE)

set(QTLIB_MOC_CPP
    PARENT_SCOPE)

set(QTLIB_RESOURCES
    PARENT_SCOPE)

set(QTLIB_SOURCES_CPP
    ${CMAKE_CURRENT_LIST_DIR}/Application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/SignalGuard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GlobalShortcut/GlobalShortcut.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GlobalShortcut/x11/X11ErrorHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GlobalShortcut/x11/X11Data.cpp
    ${CMAKE_CURRENT_LIST_DIR}/GlobalShortcut/x11/ShortcutActivator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Db/SqliteDatabase.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Db/SqlTableModelEx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Db/SqlRelationalTableModelEx.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Db/SqlNavigator.cpp
    PARENT_SCOPE)

set(QTLIB_UI_CPP
    ${CMAKE_CURRENT_LIST_DIR}/Ui/About.cpp
    PARENT_SCOPE)

if     (ENV_WIN)
    # n/a
    set(QTLIB_LIBRARIES PARENT_SCOPE)
elseif (ENV_UNIX)
    set(QTLIB_LIBRARIES X11 PARENT_SCOPE)
elseif (ENV_APPLE)
    set(QTLIB_LIBRARIES X11 PARENT_SCOPE)
else()
    message(FATAL_ERROR "${PROJECT_NAME}: unknown compiler")
endif()
#--------------------------------------------------------------------------------------------------
