#---------------------------------------------------------------------------------------------------
#
# \file  QtUtils.pri
# \brief Qt utils
#
#---------------------------------------------------------------------------------------------------


#---------------------------------------------------------------------------------------------------
# BUILD_TYPE
CONFIG(debug,   debug | release) {
    BUILD_TYPE = Debug
}
CONFIG(release, debug | release) {
    BUILD_TYPE = Release
}

# message(BUILD_TYPE - $$BUILD_TYPE)

#---------------------------------------------------------------------------------------------------
# DESTDIR_PLATFORM
DESTDIR_PLATFORM = $${QMAKE_HOST.os}_$${QMAKE_HOST.arch}_Qt$${QT_VERSION}_$${BUILD_TYPE}

message(DESTDIR_PLATFORM - $$DESTDIR_PLATFORM)

#
# DESTDIR_PLATFORM
#
# build-[XXXDb]-[Android]_for_[armeabi_v7a]_[GCC_4_8]_[Qt_5_5_0]-[Debug]
# ../build-%{CurrentProject:Name}-%{CurrentKit:FileSystemName}-%{CurrentBuild:Name}
#
# message(*******************************************************************************************)
# message(TARGET                    - $${TARGET})
# message(VERSION                   - $${VERSION})
# message(QT_VERSION                - Qt_$${QT_VERSION})
# message(QMAKE_CXX                 - $${QMAKE_CXX})
# message(QMAKE_CC                  - $${QMAKE_CC})
# message(QMAKE_HOST.arch           - $${QMAKE_HOST.arch})            # Host architecture
# message(QMAKE_HOST.os	            - $${QMAKE_HOST.os})              # Host OS
# message(QMAKE_HOST.cpu_count	    - $${QMAKE_HOST.cpu_count})       # Number of available cpus
# message(QMAKE_HOST.name	        - $${QMAKE_HOST.name})            # Host computer name
# message(QMAKE_HOST.version	    - $${QMAKE_HOST.version})         # Host OS version number
# message(QMAKE_HOST.version_string - $${QMAKE_HOST.version_string})  # Host OS version string
# message(QT_ARCH                   - $${QT_ARCH})
# message(QMAKESPEC                 - $$QMAKESPEC)
# message(OUT_PWD                   - $$OUT_PWD)
# message(*******************************************************************************************)
#
#---------------------------------------------------------------------------------------------------

