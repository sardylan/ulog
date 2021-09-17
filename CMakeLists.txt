cmake_minimum_required(VERSION 3.18)
project(ulog)

if (UNIX)
    include(GNUInstallDirs)
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(CTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(QTDIR "$ENV{QTDIR}" CACHE PATH "Qt prefix path")

find_program(QT_QMAKE_BIN
        qmake
        PATHS "${QTDIR}/bin")

find_program(QT_CMAKE_BIN
        qt-cmake
        PATHS "${QTDIR}/bin")

message(STATUS "> qmake: ${QT_QMAKE_BIN}")
message(STATUS "> qt-cmake: ${QT_CMAKE_BIN}")

if (NOT QTDIR)
    if(QT_CMAKE_BIN)
        get_filename_component(QTDIR "${QT_CMAKE_BIN}" DIRECTORY)
    endif ()
    if(QT_QMAKE_BIN)
        get_filename_component(QTDIR "${QT_QMAKE_BIN}" DIRECTORY)
    endif ()
endif ()

message(STATUS "Qt directory: ${QTDIR}")

if (NOT QTDIR)
    message(FATAL_ERROR "Qt directory not valid.")
endif ()

set(CMAKE_PREFIX_PATH "${QTDIR}/lib/cmake")

if (WIN32 OR APPLE)
    if (WIN32)
        find_program(QT_DEPLOYQT_BIN
                windeployqt
                PATHS "${QTDIR}/bin")
    elseif (APPLE)
        find_program(QT_DEPLOYQT_BIN
                macdeployqt
                PATHS "${QTDIR}/bin")
    endif ()

    if (NOT QT_DEPLOYQT_BIN)
        message(FATAL_ERROR "No Qt Deploy command found.")
    endif ()
endif ()

find_package(PkgConfig REQUIRED)

set(APPLICATION_NAME "uLog")
set(APPLICATION_VERSION_MAJOR "0")
set(APPLICATION_VERSION_MINOR "8")
set(APPLICATION_VERSION_PATCH "0")

set(ORGANIZATION_NAME "sardylan")
set(ORGANIZATION_DOMAIN "sardylan.org")

set(APPLICATION_VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}")

enable_testing()

add_subdirectory(src)

#add_subdirectory(desktop)

#include(cmake/packaging.cmake)
