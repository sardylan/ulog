find_package(Qt5 REQUIRED COMPONENTS
        Core Sql)

add_library(models
        qso.cpp qso.hpp)

target_link_libraries(models
        utilities
        Qt5::Core
        Qt5::Sql)

target_include_directories(models
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
