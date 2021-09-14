find_package(Qt5 REQUIRED COMPONENTS
        Core
        SerialPort)

pkg_check_modules(hamlib REQUIRED IMPORTED_TARGET hamlib>=4.0)
add_compile_options(${hamlib_CFLAGS})
add_link_options(${hamlib_LDFLAGS})

if (${hamlib_VERSION} VERSION_GREATER_EQUAL "4.1")
    add_definitions(-DULOG_HAMLIB_VERSION_4_1)
    if (${hamlib_VERSION} VERSION_GREATER_EQUAL "4.2")
        add_definitions(-DULOG_HAMLIB_VERSION_4_2)
        if (${hamlib_VERSION} VERSION_GREATER_EQUAL "4.3")
            add_definitions(-DULOG_HAMLIB_VERSION_4_3)
        endif ()
    endif ()
endif ()

add_library(rigctl
        rigctl.cpp rigctl.hpp
        params.cpp params.hpp
        update.cpp update.hpp
        mode.cpp mode.hpp
        models.cpp models.hpp)

target_link_libraries(rigctl
        utilities
        Qt5::Core
        Qt5::SerialPort
        PkgConfig::hamlib)

target_include_directories(rigctl
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
