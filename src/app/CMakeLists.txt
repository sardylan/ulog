find_package(Qt5 REQUIRED COMPONENTS
        Core Gui Svg Widgets Sql SerialPort)

cmake_policy(SET CMP0115 NEW)

configure_file(version.hpp.in version.hpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#set_source_files_properties(../../icons/macos/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#
#set(MACOSX_BUNDLE_EXECUTABLE_NAME qfm1000)
#set(MACOSX_BUNDLE_BUNDLE_NAME qFM1000)
#set(MACOSX_BUNDLE_ICON_FILE icon.icns)

add_executable(ulog
        WIN32 MACOSX_BUNDLE
        windows/main.cpp windows/main.hpp windows/main.ui
        windows/about.cpp windows/about.hpp windows/about.ui
        windows/qtinfo.cpp windows/qtinfo.hpp windows/qtinfo.ui
        windows/license.cpp windows/license.hpp windows/license.ui
        windows/config.cpp windows/config.hpp windows/config.ui
        status.cpp status.hpp
        config/config.cpp config/config.hpp
        config/rigctl.cpp config/rigctl.hpp
        config/serialport.cpp config/serialport.hpp
        config/manager.cpp config/manager.hpp
        config/database.cpp config/database.hpp
        ulog.cpp ulog.hpp
        main.cpp main.hpp
        database.cpp database.hpp
        version.hpp
        resources.qrc)

target_link_libraries(ulog
        utilities
        delegators
        rigctl
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::Widgets
        Qt5::Sql
        Qt5::SerialPort)

#if (WIN32)
#    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt)
#
#    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#        set(QFM1000_DEPLOYQT_TYPE "--debug")
#    else ()
#        set(QFM1000_DEPLOYQT_TYPE "--release")
#    endif ()
#
#    add_custom_command(TARGET qfm1000
#            POST_BUILD
#            COMMAND ${QT_DEPLOYQT_BIN} $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE} --dir ${PROJECT_BINARY_DIR}/windeployqt)
#
#    #install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
#    #        DESTINATION ${QFM1000_TARGET_INSTALL_DIR}
#    #        COMPONENT qtlibs)
#elseif (APPLE)
#    set_target_properties(qfm1000
#            PROPERTIES
#            MACOSX_BUNDLE_ICON_FILE ../../icons/macos/icon.icns)
#    add_custom_command(TARGET qfm1000
#            POST_BUILD
#            COMMAND ${QT_DEPLOYQT_BIN} $<TARGET_FILE:qfm1000> ${QFM1000_DEPLOYQT_TYPE})
#endif ()
#
#if (UNIX)
#    set(QFM1000_TARGET_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
#else ()
#    set(QFM1000_TARGET_INSTALL_DIR ".")
#endif ()
#
#install(TARGETS qfm1000
#        RUNTIME DESTINATION ${QFM1000_TARGET_INSTALL_DIR}
#        BUNDLE DESTINATION ${QFM1000_TARGET_INSTALL_DIR}
#        COMPONENT qfm1000)
