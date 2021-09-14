find_package(Qt5 REQUIRED COMPONENTS
        Core
        SerialPort)

pkg_check_modules(hamlib REQUIRED IMPORTED_TARGET hamlib)
add_compile_options(${hamlib_CFLAGS})
add_link_options(${hamlib_LDFLAGS})

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
